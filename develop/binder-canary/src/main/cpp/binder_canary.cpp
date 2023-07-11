
#include <jni.h>
#include <string>
#include "binder_canary.h"
#include "xdl.h"
#include <cinttypes>
#include "logging.h"
#include "jni_helper.h"
#include "global.h"
#include "system_util.h"
#include <android/api-level.h>
#include "jni_hook.h"
#include "ArtMethodHandle.h"


JavaVM *gVm = nullptr;

static struct BinderCanary_t{
    jclass javaClass = nullptr;
    jmethodID onReportMethod = nullptr;
} gBinderCanaryT;

bool hasInit = false;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    gVm = vm;
    return JNI_VERSION_1_6;
}

JNI_STATIC_METHOD(jboolean, init)(JNIEnv *env, jclass clazz, jobject monitor_config) {
    if (hasInit) {
        LOGW(">> multi call init method，already init!!");
        return false;
    }
    using namespace common;
    if (ArtMethodHandle::get().initialize(gVm)) {
        LOGW(">> init ArtMethodHandle failed");
        return false;
    }


    auto canaryClazz = (jclass) env->NewGlobalRef(env->FindClass("com/zipper/develop/binder/canary/BinderCanary"));


    init_java_monitor_type(env, monitor_config);

    gMonitorConfig.monitor_in_main_thread = get_monitor_in_main_thread(env);
    gMonitorConfig.monitor_large_data = get_config_monitor_large_data(env);
    gMonitorConfig.block_time_threshold_mills = get_config_block_time_ms(env);
    gMonitorConfig.large_data_factor = get_config_large_data_factor(env);
    hasInit = true;
    return hasInit;
}

JNI_STATIC_METHOD(jboolean, monitor)(JNIEnv *env, jclass clazz) {
    return JNI_FALSE;
}

JNI_STATIC_METHOD(jboolean, unmonitored)(JNIEnv *env, jclass clazz) {
    return JNI_FALSE;
}
