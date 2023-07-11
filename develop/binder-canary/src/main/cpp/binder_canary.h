//
// Created by Zipper on 2023/7/6.
//
#pragma once

#ifndef ANDROIDDEV_BINDER_CANARY_H
#define ANDROIDDEV_BINDER_CANARY_H

#include <android/log.h>


#define JNI_CLASS com_zipper_develop_binder_canary_BinderCanary

#define JNI_METHOD2(CLASS, FUNC)    Java_##CLASS##_##FUNC
#define JNI_METHOD1(CLASS, FUNC)    JNI_METHOD2(CLASS, FUNC)
#define JNI_METHOD(func)            JNI_METHOD1(JNI_CLASS, func)

#define JNI_STATIC_METHOD2(JNIEXPORT, RET, JNICALL, CLASS, FUNC)    extern "C" JNIEXPORT RET JNICALL Java_##CLASS##_##FUNC
#define JNI_STATIC_METHOD1(RET, CLASS, FUNC)                        JNI_STATIC_METHOD2(JNIEXPORT, RET, JNICALL, CLASS, FUNC)
#define JNI_STATIC_METHOD(RET, FUNC)                                JNI_STATIC_METHOD1(RET, JNI_CLASS, FUNC)


/// 初始化配置
/// \param[in] monitor_config 配置
JNI_STATIC_METHOD(jboolean, init)(JNIEnv *env, jclass clazz, jobject monitor_config);
/// Hook函数
JNI_STATIC_METHOD(jboolean, monitor)(JNIEnv *env, jclass clazz);
/// UnHook 函数
JNI_STATIC_METHOD(jboolean, unmonitored)(JNIEnv *env, jclass clazz);


static struct monitor_config {
    /// 仅监控主线程
    bool monitor_in_main_thread;
    /// 监控大数据
    bool monitor_large_data;
    /// 线程监控阻塞时间
    int64_t block_time_threshold_mills;
    /// 大数据计算阈值
    float large_data_factor;
} gMonitorConfig;


#endif //ANDROIDDEV_BINDER_CANARY_H

