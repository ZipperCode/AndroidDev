//
// Created by Zipper on 2023/7/14.
//
#include <string>
#include "xdl.h"
#include "jni_hook.h"
#include "bytehook.h"
#include "logging.h"
#include "monitor_filter.h"
#include "utils/jni_helper.hpp"
#include "utils/bridge_helper.hpp"


#if INTPTR_MAX == INT32_MAX
#define BINDER_SYSTEM_LIB_PATH "/system/lib/libbinder.so"
#elif INTPTR_MAX == INT64_MAX
#define BINDER_SYSTEM_LIB_PATH "/system/lib64/libbinder.so"
#else
#error "Invalid environment."
#endif



namespace binder_canary {

    void HookedCallback(
            bytehook_stub_t task_stub, int status_code, const char *caller_path_name,
            const char *sym_name, void *new_func, void *prev_func, void *arg
    ) {
        LOGI("HookedCallback, status_code: %d, caller_path_name: %s, sym_name: %s", status_code, caller_path_name, sym_name);
    }

    template<typename Return>
    Return dlSym(void *handle, const char *name){
        return (Return) xdl_sym(handle, name, nullptr);
    }

    static struct string_offset_t{
        // java.lang.String
        jclass stringClass;
        // getBytes
        jmethodID getBytesMethodId;
        // UTF-8
        jstring charsetUtf8;
    } gJString;

    static inline void assertStringInit(JNIEnv *env) {
        if (gJString.stringClass == nullptr) {
            gJString.stringClass = lsplant::JNI_NewGlobalRef(env, env->FindClass("java/lang/String"));
        }
        if (gJString.getBytesMethodId == nullptr) {
            gJString.getBytesMethodId = lsplant::JNI_GetMethodID(env, gJString.stringClass, "getBytes", "(Ljava/lang/String;)[B");
        }
        if (gJString.charsetUtf8 == nullptr) {
            auto utf8Str = env->NewStringUTF("UTF-8");
            gJString.charsetUtf8 = lsplant::JNI_NewGlobalRef(env, utf8Str);
        }
    }

    static char* convertChar(JNIEnv *env, jstring str) {
        assertStringInit(env);
        char* ret = nullptr;
        auto bytes = (jbyteArray) env->CallObjectMethod(str, gJString.getBytesMethodId, gJString.charsetUtf8);;
        jsize len = env->GetArrayLength(bytes);
        jbyte* pArr = env->GetByteArrayElements(bytes, JNI_FALSE);
        if (len > 0) {
            ret = (char*)malloc(len + 1);
            memcpy(ret, pArr, len);
            ret[len] = '\0';
        }
        env->ReleaseByteArrayElements(bytes, pArr, 0);
        return ret;
    }

    namespace bp_binder_hooker {

        bool hasInit = false;
        bool hasInitSuccess = false;
        HookFunc_t binderTransactFunc;

        IBinderGetInterfaceDescriptorType iBinderGetInterfaceDescriptorSym = nullptr;
        ParcelDataSizeType parcelDataSizeSym = nullptr;

        JavaVM *gVm = nullptr;

        class BpBinderCallInfo: public TransactCallInfo{
        public:

            BpBinderCallInfo(void *binder, uint32_t code, const Parcel &data, uint32_t flags): TransactCallInfo(){
                if (iBinderGetInterfaceDescriptorSym == nullptr) {
                    this->descriptor_ = nullptr;
                } else {
                    const String16& descriptor = iBinderGetInterfaceDescriptorSym(binder);
                    this->descriptor_ = &descriptor;
                }
                this->code_ = code;
                if (parcelDataSizeSym == nullptr) {
                    this->data_size_ = -1;
                } else {
                    this->data_size_ = parcelDataSizeSym(&data);
                }
                this->flags_ = flags;
            }

            [[nodiscard]] int flags() const override {
                return (int) flags_;
            }

            [[nodiscard]] int dataSize() const override {
                return (int) data_size_;
            }

        private:
            const String16 *descriptor_;
            uint32_t code_;
            size_t data_size_;
            uint32_t flags_;
        };

        MonitorDispatcher<BpBinderCallInfo> *monitorDispatcher = nullptr;

        /// hook 函数
        status_t HookTransactionSym(void *thiz, /* IBinder */uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags);

        void OnTransactDataLarge(const BpBinderCallInfo &callInfo);

        void onTransactBlock(const BpBinderCallInfo &callInfo);

        bool checkInit(JavaVM *vm) {
            if (hasInit) {
                return hasInitSuccess;
            }
            hasInit = true;
            XdlHandle binderHandle("libbinder.so");
            if (*binderHandle == nullptr) {
                return false;
            }

            iBinderGetInterfaceDescriptorSym = dlSym<IBinderGetInterfaceDescriptorType>(*binderHandle, "_ZNK7android8BpBinder22getInterfaceDescriptorEv");
            if (iBinderGetInterfaceDescriptorSym == nullptr) {
                LOGE("dlsym BpBinder::getInterfaceDescriptor fail");
                return false;
            }

            parcelDataSizeSym = dlSym<ParcelDataSizeType>(*binderHandle, "_ZNK7android6Parcel8dataSizeEv");

            if (parcelDataSizeSym == nullptr) {
                LOGE("dlsym Parcel::dataSize fail");
                return false;
            }
            gVm = vm;
            bytehook_set_debug(true);
            return hasInitSuccess = true;
        }

        bool Hook(JavaVM *vm) {
            if (!checkInit(vm)) {
                LOGD("Hook initFail");
                return false;
            }
            LOGD("Hook start");
            binderTransactFunc = bytehook_hook_single(
                    BINDER_SYSTEM_LIB_PATH,
                    BINDER_SYSTEM_LIB_PATH,
                    "_ZN7android8BpBinder8transactEjRKNS_6ParcelEPS1_j",
                    (void *) HookTransactionSym,
                    HookedCallback, nullptr
            );

            if (binderTransactFunc == nullptr) {
                LOGD("Hook fail binderTransactFun == nullptr");
                return false;
            }

            monitorDispatcher = new MonitorDispatcher<BpBinderCallInfo>(OnTransactDataLarge, onTransactBlock);

            return true;
        }

        bool UnHook(JavaVM *vm) {
            if (!hasInitSuccess || binderTransactFunc == nullptr) {
                return true;
            }
            int ret = bytehook_unhook(binderTransactFunc);
            if (!ret) {
                LOGE("unhook BpBinder::transact fail, ret: %d", ret);
                return false;
            }
            binderTransactFunc = nullptr;
            iBinderGetInterfaceDescriptorSym = nullptr;
            parcelDataSizeSym = nullptr;
            monitorDispatcher = nullptr;
            return false;
        }

        status_t HookTransactionSym(/* binder*/ void *thiz, uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags) {
            BYTEHOOK_STACK_SCOPE();
            BpBinderCallInfo binderCallInfo(thiz, code, data, flags);
            if (monitorDispatcher != nullptr) {
                monitorDispatcher->onTransactStart(binderCallInfo);
            }
            auto transact_ret = BYTEHOOK_CALL_PREV(HookTransactionSym, thiz, code, data, reply, flags);

            if (monitorDispatcher != nullptr) {
                monitorDispatcher->onTransactEnd(binderCallInfo);
            }

            return transact_ret;
        }

        void OnTransactDataLarge(const BpBinderCallInfo &callInfo){
            if (gVm == nullptr) {
                return;
            }
            LOGI("Binder数据过大，大小 = %d", callInfo.dataSize());
            comm::printJavaStackTrace(gVm);
        }

        void onTransactBlock(const BpBinderCallInfo &callInfo) {
            if (gVm == nullptr) {
                return;
            }
            LOGI("Binder耗时 = %ld ms, 数据大小 = %d", callInfo.costTime(), callInfo.dataSize());
            comm::printJavaStackTrace(gVm);
        }

    }


}
