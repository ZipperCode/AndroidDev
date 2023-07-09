//
// Created by Zipper on 2023/7/8.
//

#ifndef ANDROIDDEV_ARTMETHODHANDLE_H
#define ANDROIDDEV_ARTMETHODHANDLE_H

#include <jni.h>

namespace common {

    class ArtMethodHandle {
    public:
        static ArtMethodHandle &get() {
            static ArtMethodHandle handle;
            return handle;
        }

    private:
        ArtMethodHandle();

        ~ArtMethodHandle();

        ArtMethodHandle(const ArtMethodHandle &);

        ArtMethodHandle &operator=(const ArtMethodHandle &);

        static int registerNativeMethod(JNIEnv *env);

        static uint32_t getJavaOffsetMethodAccFlag(JNIEnv *env);

    public:
        bool initialize(JavaVM *vm);

        /// 获取ArtMethod结构体指针
        void* getArtMethodPtr(JNIEnv * env, jclass clazz, jmethodID methodId) ;

        void* getJNIFuncPtr(JNIEnv * env, jclass clazz, jmethodID methodId);

        /// 获取方法访问标识符
        static uint32_t getJavaMethodAccessFlag(JNIEnv *env, jclass clazz, const char *methodName, const char *sign, bool isStatic);
        bool setJNIAccessFlags(void* pArtMethod, uint32_t flags) const;
        uint32_t getJNIAccessFlags(void *pArtMethod) const;
        void addJNIAccessFlags(void* pArtMethod, uint32_t flag) const;
        bool clearJNIAccessFlag(void *pArtMethod, uint32_t flag) const;

        /**
         * 是否是Native方法
         * @param pArtMethod jmehtodID
         * @return true if native method
         */
        bool checkNativeMethod(void* pArtMethod) const;
        /// 清除FastNative注解标记的方法标识符
        bool clearFastNativeFlag(void *pArtMethod) const;

    private:
        int androidLevel_;
        JavaVM *vm_ = nullptr;
        jclass executeClass_ = nullptr;
        jfieldID artMethodField_ = nullptr;

        uint32_t artMethodOffset_ = 0;
        uint32_t artMethodSize_ = 0;
        uint32_t artMethodAccessFlagsOffset_ = 0;

    public:
        int getAndroidLevel() const{
            return this->androidLevel_;
        }

        uint32_t getArtMethodOffset() const {
            return this->artMethodOffset_;
        }

        uint32_t getArtMethodSize() {
            return this->artMethodSize_;
        }

        uint32_t getArtMethodAccessFlags() const {
            return this->artMethodAccessFlagsOffset_;
        }
    };

} // common

#endif //ANDROIDDEV_ARTMETHODHANDLE_H
