//
// Created by Zipper on 2023/7/7.
//
#pragma once
#ifndef ANDROIDDEV_COMMON_JNI_HELPER_H
#define ANDROIDDEV_COMMON_JNI_HELPER_H

#include <jni.h>

namespace common {
    static inline JNIEnv* get_env(JavaVM* vm) {
        JNIEnv* env;

        return vm->GetEnv((void **)&env, JNI_VERSION_1_6) >= 0 ? env : NULL;
    }
} // namespace common end

#endif //ANDROIDDEV_COMMON_JNI_HELPER_H
