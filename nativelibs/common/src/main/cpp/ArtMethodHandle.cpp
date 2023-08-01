//
// Created by Zipper on 2023/7/8.
//

#include <jni.h>
#include "include/ArtMethodHandle.h"
#include "include/logging.h"
#include "include/common_jni_helper.h"
#include "include/modifiers.h"
#include "include/utils/bridge_helper.hpp"

#include "utils/jni_helper.hpp"

#define JAVA_ART_METHOD "com/zipper/common/ArtMethod"
#define OFFSET_METHOD_1 "nativeStub1"
#define OFFSET_METHOD_2 "nativeStub2"
#define OFFSET_METHOD_SIGN "()V"

__attribute__((section (".mytext")))  JNICALL void native_stub1
        (JNIEnv *env, jclass obj) {
}

__attribute__((section (".mytext")))  JNICALL void native_stub2
        (JNIEnv *env, jclass obj) {
}

namespace common {
    ArtMethodHandle::ArtMethodHandle(){
        this->androidLevel_ = android_get_device_api_level();
    }

    ArtMethodHandle::~ArtMethodHandle() {
        JNIEnv *env = get_env(this->vm_);
        if (this->executeClass_ != nullptr) {
            env->DeleteGlobalRef(this->executeClass_);
            this->executeClass_ = nullptr;
        }
        this->artMethodField_ = nullptr;
        this->vm_ = nullptr;
    }


    bool ArtMethodHandle::initialize(JavaVM *vm) {
        if (this->vm_ != nullptr) {
            LOGE("ArtMethodHandle::initialize fail > already checkInit");
            return JNI_FALSE;
        }

        JNIEnv *env = get_env(vm);
        if (registerNativeMethod(env) <= 0) {
            LOGE("ArtMethodHandle::initialize fail register java native method");
            return JNI_FALSE;
        }
        jclass clazz = env->FindClass(JAVA_ART_METHOD);
        jmethodID jStubMethod1 = env->GetStaticMethodID(clazz, OFFSET_METHOD_1, OFFSET_METHOD_SIGN);
        jmethodID jStubMethod2 = env->GetStaticMethodID(clazz, OFFSET_METHOD_2, OFFSET_METHOD_SIGN);

        // 获取jni方法对应的方法内存指针
        void *nativeOffset = this->getArtMethodPtr(env, clazz, jStubMethod1);
        void *nativeOffset2 = this->getArtMethodPtr(env, clazz, jStubMethod2);
        // 得到两个相邻Native方法直接的偏移得出ArtMethod的大小
        this->artMethodSize_ = (size_t) nativeOffset2 - (size_t) nativeOffset;
        LOGD("ArtMethod >> offset1MethodId = %p, nativeOffset  = %p", jStubMethod1, nativeOffset);
        LOGD("ArtMethod >> offset2MethodId = %p, nativeOffset2 = %p", jStubMethod2, nativeOffset2);
        LOGD("ArtMethod >> ArtMethod size = %d", this->artMethodSize_);
        LOGD("ArtMethod >> native_stub1 = %p", native_stub1);
        LOGD("ArtMethod >> native_stub2 = %p", native_stub2);
        LOGD("ArtMethod >> offset = %u", ((size_t) nativeOffset - (size_t) native_stub1));
        auto jniOffset1Method = native_stub1;
        auto artMethod = reinterpret_cast<size_t *>(nativeOffset);
        for (int i = 0; i < this->artMethodSize_; i++) {
            if (reinterpret_cast<size_t *>(artMethod[i]) == reinterpret_cast<size_t*>(jniOffset1Method)) {
                this->artMethodOffset_ = i;
                break;
            }
        }

        if (this->artMethodOffset_ == 0) {
            LOGE("ArtMethodHandle::initialize fail > find ArtMethod offset error");
            return JNI_FALSE;
        }
        auto pArtMethodStartUInt32 = reinterpret_cast<uint32_t *>(nativeOffset);
        // 结构值，反射java方法的到值
        auto customAccFlag = getJavaOffsetMethodAccFlag(env);
        // 正常Java中的access
        for (int i = 0; i < this->artMethodSize_; ++i) {
            uint32_t value = *(pArtMethodStartUInt32 + i);
            if (value == customAccFlag){
                // 得到访问标识符的位置
                this->artMethodAccessFlagsOffset_ = i;
                break;
            }
        }
        if (this->artMethodAccessFlagsOffset_ <= 0){
            if (this->androidLevel_ >= __ANDROID_API_N__) {
                this->artMethodAccessFlagsOffset_ = 4 / sizeof(uint32_t);
            }else if (this->androidLevel_ == __ANDROID_API_M__){
                this->artMethodAccessFlagsOffset_ = 12 / sizeof(uint32_t);
            }else if (this->androidLevel_ == __ANDROID_API_L_MR1__){
                this->artMethodAccessFlagsOffset_ = 20 / sizeof(uint32_t);
            }else if (this->androidLevel_ == __ANDROID_API_L__){
                this->artMethodAccessFlagsOffset_ = 56 / sizeof(uint32_t);
            }
        }

        this->vm_ = vm;
        return JNI_TRUE;
    }


    int ArtMethodHandle::registerNativeMethod(JNIEnv *env) {
        jclass clazz = env->FindClass(JAVA_ART_METHOD);
        JNINativeMethod gNativeArtMethods[] = {
                {OFFSET_METHOD_1, OFFSET_METHOD_SIGN, (void *) native_stub1},
                {OFFSET_METHOD_2, OFFSET_METHOD_SIGN, (void *) native_stub2},
        };
        if (registerNatives(env, clazz, gNativeArtMethods) < 0) {
            LOGE("jni register error.");
            return JNI_FALSE;
        }

        return JNI_TRUE;
    }

    void *ArtMethodHandle::getArtMethodPtr(JNIEnv *env, jclass clazz, jmethodID methodId) {
        // Android11 后获取artMethod指针通过反射，11以前methodId就是ArtMethod指针
        if (this->androidLevel_ >= __ANDROID_API_Q__) {
            if (this->executeClass_ == nullptr || this->artMethodField_ == nullptr){
                LOGE("ArtMethodHandle::getNativeMethod > 需要使用反射获取ArtMethodId");
                jclass executable = env->FindClass("java/lang/reflect/Executable");
                this->executeClass_ = reinterpret_cast<jclass>(env->NewGlobalRef(executable));
                jfieldID artId = env->GetFieldID(executable, "artMethod", "J");
                this->artMethodField_ = artId;
            }

            jobject method = env->ToReflectedMethod(clazz, methodId, true);
            return reinterpret_cast<void *>(env->GetLongField(method, this->artMethodField_));
        } else {
            // ALOGE("不需要使用反射获取ArtMethodId")
            return methodId;
        }
    }

    void *ArtMethodHandle::getJNIFuncPtr(JNIEnv *env, jclass clazz, jmethodID methodId) {
        auto pArtMethod = this->getArtMethodPtr(env, clazz, methodId);
        return reinterpret_cast<void *>((uint32_t*) pArtMethod + this->artMethodOffset_);
    }


    uint32_t ArtMethodHandle::getJavaMethodAccessFlag(JNIEnv *env, jclass clazz, const char *methodName, const char *sign, bool isStatic) {
        uint32_t accFlag = 0;
        jclass executable = env->FindClass("java/lang/reflect/Executable");
        jfieldID accFlagFieldId = env->GetFieldID(executable, "accessFlags", "I");
        jmethodID offset1MethodId = env->GetStaticMethodID(clazz, methodName, sign);
        // Method object
        jobject methodObj = env->ToReflectedMethod(clazz, offset1MethodId, isStatic);
        accFlag = env->GetIntField(methodObj, accFlagFieldId);
        return accFlag;
    }

    uint32_t ArtMethodHandle::getJavaOffsetMethodAccFlag(JNIEnv *env) {
        jclass javaArtMethodCls = env->FindClass(JAVA_ART_METHOD);
        return getJavaMethodAccessFlag(env, javaArtMethodCls, OFFSET_METHOD_1, OFFSET_METHOD_SIGN, true);
    }

    bool ArtMethodHandle::setJNIAccessFlags(void *pArtMethod, uint32_t flags) const {
        return *(reinterpret_cast<uint32_t *>(pArtMethod) + this->artMethodAccessFlagsOffset_) = flags;;
    }

    uint32_t ArtMethodHandle::getJNIAccessFlags(void *pArtMethod) const {
        return *(reinterpret_cast<uint32_t *>(pArtMethod) + this->artMethodAccessFlagsOffset_);
    }


    void ArtMethodHandle::addJNIAccessFlags(void *pArtMethod, uint32_t flag) const {
        uint32_t oldFlags = this->getJNIAccessFlags(pArtMethod);
        uint32_t newFlags = oldFlags | flag;
        this->setJNIAccessFlags(pArtMethod, newFlags);
    }


    bool ArtMethodHandle::clearJNIAccessFlag(void *pArtMethod, uint32_t flag) const {
        uint32_t old_flag = this->getJNIAccessFlags(pArtMethod);
        uint32_t new_flag = old_flag & ~flag;
        return new_flag != old_flag && setJNIAccessFlags(pArtMethod, new_flag);
    }

    bool ArtMethodHandle::checkNativeMethod(void *pArtMethod) const {
        uint32_t oldFlags = this->getJNIAccessFlags(pArtMethod);
        return (oldFlags & kAccNative) != 0;
    }

    bool ArtMethodHandle::clearFastNativeFlag(void *pArtMethod) const {
        // FastNative
        return this->androidLevel_ < __ANDROID_API_P__ &&
               this->clearJNIAccessFlag(pArtMethod, kAccFastNative);
    }



} // common