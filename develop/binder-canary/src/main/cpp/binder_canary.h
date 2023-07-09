//
// Created by Zipper on 2023/7/6.
//
#pragma once

#ifndef ANDROIDDEV_BINDER_CANARY_H
#define ANDROIDDEV_BINDER_CANARY_H
#include <android/log.h>


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



extern "C" JNIEXPORT jboolean JNICALL
com_zipper_develop_binder_canary_BinderCanary_init(JNIEnv *env, jclass clazz, jobject monitor_config);



int test(int a, int b);


#endif //ANDROIDDEV_BINDER_CANARY_H
