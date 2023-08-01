//
// Created by Zipper on 2023/8/1.
//
#pragma once

#ifndef ANDROIDDEV_BRIDGE_HELPER_HPP
#define ANDROIDDEV_BRIDGE_HELPER_HPP

#include <jni.h>
#include "jni_helper.hpp"

namespace comm {


    static struct java_trace_offset_t {
        // android.util.Log
        jclass logClass = nullptr;
        // android.util.Log
        jmethodID getStackTraceStringMethodId = nullptr;
        // java.lang.Throwable
        jclass throwableClass = nullptr;
        jmethodID throwConstructorMethodId = nullptr;
    } gJTrace;

    static struct string_offset_t{
        // java.lang.String
        jclass stringClass;
        // getBytes
        jmethodID getBytesMethodId;
        // UTF-8
        jstring charsetUtf8;
    } gJString;

    static char* convertChar(JNIEnv *env, jstring str);

    static inline void assertJavaTrace(JNIEnv *env) {
        if (gJTrace.logClass == nullptr || gJTrace.getStackTraceStringMethodId == nullptr) {
            auto logClass = env->FindClass("android/util/Log");
            gJTrace.logClass = lsplant::JNI_NewGlobalRef(env, logClass);
            gJTrace.getStackTraceStringMethodId = env->GetStaticMethodID(logClass, "getStackTraceString", "(Ljava/lang/Throwable;)Ljava/lang/String;");
        }
        if (gJTrace.throwableClass == nullptr || gJTrace.throwConstructorMethodId == nullptr) {
            gJTrace.throwableClass = lsplant::JNI_NewGlobalRef(env, env->FindClass("java/lang/Throwable"));
            gJTrace.throwConstructorMethodId = env->GetMethodID(gJTrace.throwableClass, "<init>", "()V");
        }
    }

    static inline jobject getThrowableObject(JNIEnv *env) {
        assertJavaTrace(env);
        return env->NewObject(gJTrace.throwableClass, gJTrace.throwConstructorMethodId);
    }

    static inline jstring getStackTrace(JNIEnv *env) {
        assertJavaTrace(env);
        auto throwableObj = getThrowableObject(env);
        jstring stackTraceStr = static_cast<jstring>(env->CallStaticObjectMethod(gJTrace.logClass, gJTrace.getStackTraceStringMethodId, throwableObj));
        return stackTraceStr;
    }

    static inline void printJavaStackTrace(JavaVM *vm){
        JNIEnv *env;
        if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK){
            return;
        }
        auto traceStr = getStackTrace(env);
        LOGE("\n%s", convertChar(env, traceStr));
    }

    static inline void printJavaStackTrace(JNIEnv *env){
        auto traceStr = getStackTrace(env);
        LOGE("\n%s", convertChar(env, traceStr));
    }

    static inline void assertStringInit(JNIEnv *env) {
        if (gJString.stringClass == nullptr) {
            gJString.stringClass = lsplant::JNI_NewGlobalRef(env, env->FindClass("java/lang/String"));
        }
        if (gJString.getBytesMethodId == nullptr) {
            gJString.getBytesMethodId = lsplant::JNI_GetMethodID(env, gJString.stringClass, "getBytes", "(Ljava/lang/String;)[B");
        }
        if (gJString.charsetUtf8 == nullptr) {
            auto utf8Str = env->NewStringUTF("UTF-8");
            gJString.charsetUtf8 = lsplant::JNI_NewGlobalRef(env, utf8Str);
        }
    }

    static char* convertChar(JNIEnv *env, jstring str) {
        assertStringInit(env);
        char* ret = nullptr;
        auto bytes = (jbyteArray) env->CallObjectMethod(str, gJString.getBytesMethodId, gJString.charsetUtf8);;
        jsize len = env->GetArrayLength(bytes);
        jbyte* pArr = env->GetByteArrayElements(bytes, JNI_FALSE);
        if (len > 0) {
            ret = (char*)malloc(len + 1);
            memcpy(ret, pArr, len);
            ret[len] = '\0';
        }
        env->ReleaseByteArrayElements(bytes, pArr, 0);
        return ret;
    }
}


#endif //ANDROIDDEV_BRIDGE_HELPER_HPP
