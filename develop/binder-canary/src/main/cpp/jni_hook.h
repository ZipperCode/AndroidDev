//
// Created by Zipper on 2023/7/7.
//

#ifndef ANDROIDDEV_BINDER_JNI_HOOK_H
#define ANDROIDDEV_BINDER_JNI_HOOK_H

#include <jni.h>
#include "system_util.h"
#include <android/api-level.h>
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


#endif //ANDROIDDEV_BINDER_JNI_HOOK_H
