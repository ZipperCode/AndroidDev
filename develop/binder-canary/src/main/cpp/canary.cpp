
#include <jni.h>
#include <string>
#include "binder_canary.h"
#include "include/xdl/xdl.h"
#include <inttypes.h>

#define LOG(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "xdl_tag", fmt, ##__VA_ARGS__)

int test(int a, int b) {
    return a + b;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_zipper_develop_binder_canary_NativeLib_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    const char* filename = "/system/lib64/libc++.so";
    void* handle = xdl_open(filename, XDL_DEFAULT);
    LOG(">>> xdl_open(%s) : handle %" PRIxPTR, filename, (uintptr_t)handle);
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}