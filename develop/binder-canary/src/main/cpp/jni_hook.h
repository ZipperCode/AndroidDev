//
// Created by Zipper on 2023/7/7.
//

#ifndef ANDROIDDEV_BINDER_JNI_HOOK_H
#define ANDROIDDEV_BINDER_JNI_HOOK_H

#include <jni.h>
#include "system_util.h"
#include "monitor_filter.h"
#include <android/api-level.h>
#include <ostream>
#include "xdl.h"
/**
 * JNI Hook 定义 将指定方法替换为代理方法
 *      HOOK_JNI(void*, fun, int, int)
 * =>   void* fun(int, int);
 * =>   void* new_fun(int, int)
 */
#define HOOK_JNI(ret, func, ...) \
  ret (*orig_##func)(__VA_ARGS__); \
  ret new_##func(__VA_ARGS__)
#define HOOK_PTR(name) reinterpret_cast<void *>(new_##name), \
    reinterpret_cast<void **>(&orig_##name), false

#define STUB_METHOD1 "nativeStub1"
#define STUB_METHOD2 "nativeStub2"
#define METHOD_SIGN "()V"
#define DISALLOW_COPY_AND_ASSIGN(TypeName)                                                         \
    TypeName(const TypeName &) = delete;                                                           \
    void operator=(const TypeName &) = delete

#define CREATE_FUNC_SYMBOL_ENTRY(ret, func, ...)                                                   \
    typedef ret (*func##Type)(__VA_ARGS__);                                                        \
    inline static ret (*func##Sym)(__VA_ARGS__);                                                   \
    inline static ret func(__VA_ARGS__)

namespace {
    // https://cs.android.com/android/platform/superproject/+/refs/heads/master:frameworks/native/libs/binder/include/binder/Parcel.h
    class Parcel;

    class SharedBuffer {
        SharedBuffer(const SharedBuffer&);
        SharedBuffer& operator = (const SharedBuffer&);
    public:
        static inline SharedBuffer * bufferFromData(void* data) {
            return data ? static_cast<SharedBuffer *>(data)-1 : nullptr;
        }

        static inline size_t sizeFromData(void* data) {
            return data ? bufferFromData(data)->mSize : 0;
        }
    private:
        mutable std::atomic<int32_t> mRefs;
        size_t mSize;
        uint32_t mReserved;
    public:
        uint32_t mClientMetadata;
    };

    // https://cs.android.com/android/platform/superproject/+/refs/heads/master:system/core/libutils/include/utils/String16.h
    class String16 {
    public:
        /**
         * A flag indicating the type of underlying buffer.
         */
        static constexpr uint32_t kIsSharedBufferAllocated = 0x80000000;

        [[nodiscard]] inline const char16_t *string() const {
            return mString;
        }

        [[nodiscard]] inline const bool isStaticString() const {
            const uint32_t *p = reinterpret_cast<const uint32_t *>(mString);
            return (*(p - 1) & kIsSharedBufferAllocated) == 0;
        }

        [[nodiscard]] inline const size_t staticStringSize() const {
            const uint32_t *p = reinterpret_cast<const uint32_t *>(mString);
            return static_cast<size_t>(*(p - 1));
        }

        inline const size_t size() const {
            if (isStaticString()) {
                return staticStringSize();
            } else {
                return SharedBuffer::sizeFromData((void *) mString) / sizeof(char16_t) - 1;
            }
        }

    private:
        const char16_t *mString;
    };

}

namespace binder_canary {

    typedef void *HookFunc_t;

    typedef int32_t status_t;

    /// size_t dataSize() const;
    CREATE_FUNC_SYMBOL_ENTRY(size_t, ParcelDataSize, const Parcel *);
    /// virtual const String16& getInterfaceDescriptor() const = 0;
    CREATE_FUNC_SYMBOL_ENTRY(String16, IBinderGetInterfaceDescriptor, /* IBinder* */ const void *);
    /// size_t size() const;
    CREATE_FUNC_SYMBOL_ENTRY(size_t, String16Size, const String16 *);

    namespace bp_binder_hooker {


        bool checkInit(JavaVM *vm);
        bool Hook(JavaVM *vm);
        bool UnHook(JavaVM *vm);

    }

    namespace binder_proxy_hooker {
        class BinderProxyCallInfo;

        bool checkInit(JavaVM *vm);

        bool Hook(JavaVM *vm);

        bool UnHook(JavaVM *vm);


        class BinderProxyCallInfo : public TransactCallInfo {
        public:
            BinderProxyCallInfo(
                    JNIEnv *env, jobject obj, jint code, jobject data_obj, jint flags
            ) : TransactCallInfo(),
                env_(env), obj_(obj), code_(code), data_obj_(data_obj), flags_(flags) {}

        public:
            [[nodiscard]] int flags() const override {
                return 0;
            }

            [[nodiscard]] int dataSize() const override {
                return 0;
            }

        public:
            JNIEnv *env_;
            jobject obj_;
            jint code_;
            jobject data_obj_;
            jint flags_;
        };
    }

    class XdlHandle {
    public:
        XdlHandle(const char *name) : lib_handle_(xdl_open(name, XDL_DEFAULT)) {}

        ~XdlHandle() {
            if (lib_handle_ != nullptr) {
                xdl_close(lib_handle_);
            }
            lib_handle_ = nullptr;
        }

    public:
        void *operator*() {
            return lib_handle_;
        }

    private:
        void *lib_handle_;

        DISALLOW_COPY_AND_ASSIGN(XdlHandle);
    };
}

#endif //ANDROIDDEV_BINDER_JNI_HOOK_H
