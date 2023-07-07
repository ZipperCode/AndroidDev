#pragma once

#include <android/api-level.h>
#include <jni.h>
#include <string>
#include <list>
#include <sys/system_properties.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName)                                                         \
    TypeName(const TypeName &) = delete;                                                           \
    void operator=(const TypeName &) = delete

namespace common {

    enum class Arch {
        kArm,
        kArm64,
        kX86,
        kX86_64,
        kRiscv64,
        kUnsupported
    };

    // 获取系统架构
    constexpr inline Arch

    GetArch() {
#if defined(__i386__)
        return Arch::kX86;
#elif defined(__x86_64__)
        return Arch::kX86_64;
#elif defined(__arm__)
        return Arch::kArm;
#elif defined(__aarch64__)
        return Arch::kArm64;
#elif defined(__riscv)
        return Arch::kRiscv64;
#else
        return Arch::kUnsupported;
#endif
    }

    /// 获取当前架构信息
    static constexpr auto arch = GetArch();

    static inline auto GetAndroidApiLevel() {
        static auto kApiLevel = []() -> int {
            std::array<char, PROP_VALUE_MAX> prop_value;
            __system_property_get("ro.build.version.sdk", prop_value.data());
            int base = atoi(prop_value.data());
            __system_property_get("ro.build.version.preview_sdk", prop_value.data());
            return base + atoi(prop_value.data());
        }();
        return kApiLevel;
    }

    template <typename T>
    concept JObject = std::is_base_of_v<std::remove_pointer_t<_jobject>, std::remove_pointer_t<T>>;


    template<JObject T>
    class ScopedLocalRef {
    public:
        using BaseType [[maybe_unused]] = T;

        ScopedLocalRef(JNIEnv *env, T local_ref) : env_(env), local_ref_(nullptr) {
            reset(local_ref);
        }

        ScopedLocalRef(ScopedLocalRef &&s) noexcept : ScopedLocalRef(s.env_, s.release()) {}

        template <JObject U>
        ScopedLocalRef(ScopedLocalRef<U> &&s) noexcept : ScopedLocalRef(s.env_, (T)s.release()) {}

        explicit ScopedLocalRef(JNIEnv *env) noexcept : ScopedLocalRef(env, T{nullptr}) {}

        ~ScopedLocalRef() {
            reset();
        }

        void reset(T ptr = nullptr) {
            if (ptr != local_ref_) {
                if (local_ref_ != nullptr) {
                    env_->DeleteLocalRef(local_ref_);
                }
                local_ref_ = ptr;
            }
        }

        [[nodiscard]] T release() {
            T local_ref = local_ref_;
            local_ref_ = nullptr;
            return local_ref;
        }

        T get() const {
            return local_ref_;
        }

        operator T() const {
            return local_ref_;
        }

        ScopedLocalRef &operator=(ScopedLocalRef &&s) noexcept {
            reset(s.release());
            env_ = s.env_;
            return *this;
        }

        operator bool() const {
            return local_ref_ != nullptr;
        }

        friend class JUTFString;

    private:
        JNIEnv *env_;
        T local_ref_;
        DISALLOW_COPY_AND_ASSIGN(ScopedLocalRef);
    };


    inline ScopedLocalRef<jstring> ClearException(JNIEnv *env) {
        if (auto exception = env->ExceptionOccurred()) {
            env->ExceptionClear();
            static jclass log = (jclass) env->NewGlobalRef(env->FindClass("android/util/Log"));
            static jmethodID toString = env->GetStaticMethodID(
                    log, "getStackTraceString", "(Ljava/lang/Throwable;)Ljava/lang/String;");
            auto str = (jstring) env->CallStaticObjectMethod(log, toString, exception);
            env->DeleteLocalRef(exception);
            return {env, str};
        }
        return {env, nullptr};
    }
    /// jstring的包装，更好的获取char*
    class JUTFString {
        inline JUTFString(JNIEnv *env, jstring jstr) : JUTFString(env, jstr, nullptr) {}

        inline JUTFString(const ScopedLocalRef<jstring> &jstr)
                : JUTFString(jstr.env_, jstr.local_ref_, nullptr) {}

        inline JUTFString(JNIEnv *env, jstring jstr, const char *default_cstr)
                : env_(env), jstr_(jstr) {
            if (env_ && jstr_)
                cstr_ = env_->GetStringUTFChars(jstr, nullptr);
            else
                cstr_ = default_cstr;
        }

        inline operator const char *() const { return cstr_; }

        inline operator const std::string() const { return cstr_; }

        inline operator const bool() const { return cstr_ != nullptr; }

        inline auto get() const { return cstr_; }

        inline ~JUTFString() {
            if (env_ && jstr_) env_->ReleaseStringUTFChars(jstr_, cstr_);
        }

        JUTFString(JUTFString &&other)
                : env_(std::move(other.env_)),
                  jstr_(std::move(other.jstr_)),
                  cstr_(std::move(other.cstr_)) {
            other.cstr_ = nullptr;
        }

        JUTFString &operator=(JUTFString &&other) {
            if (&other != this) {
                env_ = std::move(other.env_);
                jstr_ = std::move(other.jstr_);
                cstr_ = std::move(other.cstr_);
                other.cstr_ = nullptr;
            }
            return *this;
        }

    private:
        JNIEnv *env_;
        jstring jstr_;
        const char *cstr_;

        JUTFString(const JUTFString &) = delete;

        JUTFString &operator=(const JUTFString &) = delete;
    };
}


