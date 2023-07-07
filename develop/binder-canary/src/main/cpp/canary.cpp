
#include <jni.h>
#include <string>
#include "binder_canary.h"
#include "xdl.h"
#include <cinttypes>
#include "logging.h"


int test(int a, int b) {
    return a + b;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_zipper_develop_binder_canary_NativeLib_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    const char* filename = "/system/lib64/libc++.so";
    void* handle = xdl_open(filename, XDL_DEFAULT);
    LOGD(">>> xdl_open(%s) : handle %" PRIxPTR, filename, (uintptr_t)handle);
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}