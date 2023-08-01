//
// Created by Zipper on 2023/7/7.
//
#pragma once
#ifndef ANDROIDDEV_COMMON_JNI_HELPER_H
#define ANDROIDDEV_COMMON_JNI_HELPER_H

#include <jni.h>
#include <string>
#include <array>

namespace common {
    /// 从JavaVm中获取JNIEnv指针对象
    static inline JNIEnv *get_env(JavaVM *vm) {
        JNIEnv *env;

        return vm->GetEnv((void **) &env, JNI_VERSION_1_6) >= 0 ? env : NULL;
    }
    /// 注册JNI函数
    static inline int registerNatives(JNIEnv *env, const jclass clazz, const JNINativeMethod *methods) {
        // LOGD("func size = %u", sizeof(*methods) / sizeof(methods[0]));
        return env->RegisterNatives(clazz, methods, sizeof(*methods) / sizeof(methods[0]));
    }
} // namespace common end

#endif //ANDROIDDEV_COMMON_JNI_HELPER_H
