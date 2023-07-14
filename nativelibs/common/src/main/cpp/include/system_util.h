#pragma once

#include <android/api-level.h>
#include <jni.h>
#include <string>
#include <array>
#include <sys/system_properties.h>
#include <unistd.h>

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

    static const auto sApiLevel = GetAndroidApiLevel();


    static inline long GetCurrentTimeMills() {
        timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }

    static inline bool IsMainThread() {
        return getpid() == gettid();
    }
}


