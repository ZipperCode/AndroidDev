//
// Created by Zipper on 2023/7/7.
//

#ifndef ANDROIDDEV_BINDER_GLOBAL_H
#define ANDROIDDEV_BINDER_GLOBAL_H

namespace binder_canary {

    /// java Bean结构
    static struct java_monitor_config_offset_t {
        [[maybe_unused]] jclass clazz;
        jobject target;
        jfieldID monitor_in_thread_field;
        jfieldID monitor_large_data_field;
        jfieldID block_time_threshold_mills_field;
        jfieldID large_data_factor_field;
    } gMonitorConfigOffsetT;

    /// 全局配置
    static struct monitor_config {
        /// 仅监控主线程
        bool monitor_in_main_thread = true;
        /// 监控大数据
        bool monitor_large_data = false;
        /// 线程监控阻塞时间
        int64_t block_time_threshold_mills = 50;
        /// 大数据计算阈值
        float large_data_factor = 0.75;
    } gMonitorConfig;

    static inline void init_java_monitor_type(JNIEnv *env, jobject target) {
        auto configClazz = env->FindClass("com/zipper/develop/binder/canary/MonitorConfig");
        gMonitorConfigOffsetT.clazz = reinterpret_cast<jclass>(
                env->NewGlobalRef(configClazz)
        );

        gMonitorConfigOffsetT.target = env->NewGlobalRef(target);
        gMonitorConfigOffsetT.monitor_in_thread_field = env->GetFieldID(configClazz, "monitorInMainThread", "Z");
        gMonitorConfigOffsetT.monitor_large_data_field = env->GetFieldID(configClazz, "monitorLargeData", "Z");
        gMonitorConfigOffsetT.block_time_threshold_mills_field = env->GetFieldID(configClazz, "blockTimeThresholdMils", "J");
        gMonitorConfigOffsetT.large_data_factor_field = env->GetFieldID(configClazz, "largeDataFactor", "F");
    }

    static inline bool get_monitor_in_main_thread(JNIEnv *env) {
        return env->GetBooleanField(
                gMonitorConfigOffsetT.target,
                gMonitorConfigOffsetT.monitor_in_thread_field
        );
    }

    static inline bool get_config_monitor_large_data(JNIEnv *env) {
        return env->GetBooleanField(
                gMonitorConfigOffsetT.target,
                gMonitorConfigOffsetT.monitor_large_data_field
        );
    }

    static inline jlong get_config_block_time_ms(JNIEnv *env) {
        return env->GetLongField(
                gMonitorConfigOffsetT.target,
                gMonitorConfigOffsetT.block_time_threshold_mills_field
        );
    }

    static inline jfloat get_config_large_data_factor(JNIEnv *env) {
        return env->GetFloatField(
                gMonitorConfigOffsetT.target,
                gMonitorConfigOffsetT.large_data_factor_field
        );
    }
}


#endif //ANDROIDDEV_BINDER_GLOBAL_H
