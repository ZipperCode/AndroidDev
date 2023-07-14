//
// Created by Zipper on 2023/7/14.
//
#include "jni_hook.h"
#include "bytehook.h"
#include "logging.h"
#include "monitor_filter.h"
#include "xdl.h"

#define BINDER_SYSTEM_LIB_PATH "/system/lib64/libbinder.so"

namespace binder_canary {

    class BpBinderCallInfo: public TransactCallInfo{
    public:

        BpBinderCallInfo(void *binder, uint32_t code, const Parcel &data, uint32_t flags) {}

        [[nodiscard]] int flags() const override {
            return 0;
        }

        [[nodiscard]] int dataSize() const override {
            return 0;
        }
    private:

        uint32_t flags_;
    };

    void HookedCallback(
            bytehook_stub_t, int status_code,
            const char *caller_path_name, const char *sym_name,
            void *, void *,
            void *
    ) {
        LOGI("HookedCallback, status_code: %d, caller_path_name: %s, sym_name: %s", status_code, caller_path_name, sym_name);
    }

    bool BpBinderHooker::hasInit_ = false;
    bool BpBinderHooker::hasInitSuccess_ = false;
    ParcelDataSizeType BpBinderHooker::parcelDataSizeSym_ = nullptr;
    IBinderGetInterfaceDescriptorType BpBinderHooker::iBinderGetInterfaceDescriptorSym = nullptr;

    status_t HookTransactionSym(void *thiz, /* IBinder */uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags);

    template<typename Return>
    Return dlsym(void *handle, const char *name){
        return (Return) xdl_sym(handle, name, nullptr);
    }

    bool BpBinderHooker::checkInit(JavaVM *vm) {
        if (hasInit_) {
            return hasInitSuccess_;
        }
        hasInit_ = true;
        XdlHandle binderHandle("libbinder.so");
        if (*binderHandle == nullptr) {
            return false;
        }

        iBinderGetInterfaceDescriptorSym = dlsym<IBinderGetInterfaceDescriptorType>(*binderHandle, "_ZNK7android8BpBinder22getInterfaceDescriptorEv");
        if (iBinderGetInterfaceDescriptorSym == nullptr) {
            LOGE("dlsym BpBinder::getInterfaceDescriptor fail");
            return false;
        }

        parcelDataSizeSym_ = dlsym<ParcelDataSizeType>(&binderHandle, "_ZNK7android6Parcel8dataSizeEv");

        if (parcelDataSizeSym_ == nullptr) {
            LOGE("dlsym Parcel::dataSize fail");
            return false;
        }

        XdlHandle utils_handle("libutils.so");


        hasInitSuccess_ = true;
        return false;
    }

    bool BpBinderHooker::Hook(JavaVM *vm) {
        if (!checkInit(vm)) {
            return false;
        }

        binderTransactFunc_ = bytehook_hook_single(
                BINDER_SYSTEM_LIB_PATH,
                BINDER_SYSTEM_LIB_PATH,
                "_ZN7android8BpBinder8transactEjRKNS_6ParcelEPS1_j",
                (void *) HookTransactionSym,
                HookedCallback, nullptr
        );


    }

    bool BpBinderHooker::UnHook(JavaVM *vm) {

    }

    status_t HookTransactionSym(void *thiz, uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags) {
        BYTEHOOK_STACK_SCOPE();

        auto transact_ret = BYTEHOOK_CALL_PREV(HookTransactionSym, thiz, code, data, reply, flags);

        return transact_ret;
    }


    bool BinderProxyHooker::checkInit(JavaVM *vm) {
        return false;
    }

    bool BinderProxyHooker::Hook(JavaVM *vm) {
        return false;
    }

    bool BinderProxyHooker::UnHook(JavaVM *vm) {
        return false;
    }


}
