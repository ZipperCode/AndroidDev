//
// Created by Zipper on 2023/7/7.
//
#pragma once
#ifndef ANDROIDDEV_COMMON_JNI_HELPER_H
#define ANDROIDDEV_COMMON_JNI_HELPER_H

#include <jni.h>
#include <string>
#include <array>

#define DISALLOW_COPY_AND_ASSIGN(TypeName)                                                         \
    TypeName(const TypeName &) = delete;                                                           \
    void operator=(const TypeName &) = delete

namespace common {
    /// 指定class是否是一个模板类实例
    template<class, template<class, class...> class>
    struct is_instance : public std::false_type {
    };

    /// 指定多个class Ts 是否是一个模板类U的实例
    template<class... Ts, template<class, class...> class U>
    struct is_instance<U<Ts...>, U> : public std::true_type {
    };

    /// 指定T是否是一个模板类U的实例
    template<class T, template<class, class...> class U>
    inline constexpr bool is_instance_v = is_instance<T, U>::value;

    /// 从JavaVm中获取JNIEnv指针对象
    static inline JNIEnv *get_env(JavaVM *vm) {
        JNIEnv *env;

        return vm->GetEnv((void **) &env, JNI_VERSION_1_6) >= 0 ? env : NULL;
    }
    /// 注册JNI函数
    static inline int registerNatives(JNIEnv *env, const jclass clazz, const JNINativeMethod *methods) {
        LOGD("func size = %d", sizeof(*methods) / sizeof(methods[0]));
        return env->RegisterNatives(clazz, methods, sizeof(*methods) / sizeof(methods[0]));
    }

    /// concept : c++20 关键字
    /// concept声明一个类型对泛型T进行限制
    /// 判断一个类型T是否是[_jobject]类型
    template<typename T>
    concept JObject = std::is_base_of_v<std::remove_pointer_t<_jobject>, std::remove_pointer_t<T>>;

    /// 泛型限定了T只能是jobject类型
    /// 包装一个jobject对象，对象能在代码范围内
    template<JObject T>
    class ScopedLocalRef {
    public:
        using BaseType [[maybe_unused]] = T;

        ScopedLocalRef(JNIEnv *env, T local_ref) : env_(env), local_ref_(nullptr) {
            reset(local_ref);
        }

        ScopedLocalRef(ScopedLocalRef &&s) noexcept: ScopedLocalRef(s.env_, s.release()) {}

        template<JObject U>
        ScopedLocalRef(ScopedLocalRef<U> &&s) noexcept : ScopedLocalRef(s.env_, (T) s.release()) {}

        explicit ScopedLocalRef(JNIEnv *env) noexcept: ScopedLocalRef(env, T{nullptr}) {}

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

    /// \param[in] x 两个&&表示去右值引用
    template<typename T>
    [[maybe_unused]] inline auto UnwrapScope(T &&x) {
        if constexpr (std::is_same_v<std::decay_t<T>, std::string_view>)
            // string_view 则去除其内部char类型
            return x.data();
        else if constexpr (is_instance_v<std::decay_t<T>, ScopedLocalRef>)
            // ScopedLocalRef 则去除其包装的类型
            return x.get();
        else
            // 确保参数x引用类型不发生改变
            return std::forward<T>(x);
    }

    template<typename T>
    [[maybe_unused]] inline auto WrapScope(JNIEnv *env, T &&x) {
        if constexpr (std::is_convertible_v<T, _jobject *>) {
            // 如果是一个jobject类型则包装为一个ScopedLocalRef，通过转发保证x的引用类型类型
            return ScopedLocalRef(env, std::forward<T>(x));
        } else
            return x;
    }

    template<typename... T, size_t... I>
    [[maybe_unused]] inline auto WrapScope(JNIEnv *env, std::tuple<T...> &&x,
                                           std::index_sequence<I...>) {
        return std::make_tuple(WrapScope(env, std::forward<T>(std::get<I>(x)))...);
    }

    template<typename... T>
    [[maybe_unused]] inline auto WrapScope(JNIEnv *env, std::tuple<T...> &&x) {
        return WrapScope(env, std::forward<std::tuple<T...>>(x),
                         std::make_index_sequence<sizeof...(T)>());
    }

    /// jstring的包装，更好的获取char*
    class JUTFString {
    public:
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

    template <typename Func, typename... Args>
    requires(std::is_function_v<Func>)
    [[maybe_unused]] inline auto JNI_SafeInvoke(JNIEnv *env, Func JNIEnv::*f, Args &&...args) {
        struct finally {
            finally(JNIEnv *env) : env_(env) {}

            ~finally() {
                if (auto exception = ClearException(env_)) {
                    __android_log_print(ANDROID_LOG_ERROR,
#ifdef LOG_TAG
                                        LOG_TAG,
#else
                            "JNIHelper",
#endif
                                        "%s", JUTFString(env_, exception.get()).get());
                }
            }

            JNIEnv *env_;
        } _(env);

        if constexpr (!std::is_same_v<void,
                std::invoke_result_t<Func, decltype(UnwrapScope(
                        std::forward<Args>(args)))...>>)
            return WrapScope(env, (env->*f)(UnwrapScope(std::forward<Args>(args))...));
        else
            (env->*f)(UnwrapScope(std::forward<Args>(args))...);
    }

} // namespace common end

#endif //ANDROIDDEV_COMMON_JNI_HELPER_H
