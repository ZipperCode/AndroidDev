
#include <jni.h>
#include <string>
#include "binder_canary.h"
#include "xdl.h"
#include <cinttypes>
#include "logging.h"
#include "global.h"
#include "system_util.h"
#include <android/api-level.h>
#include <bytehook.h>
#include "jni_hook.h"
#include "utils/jni_helper.hpp"



JavaVM *gVm = nullptr;
using namespace binder_canary;

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
        LOGW(">> multi call init method，already checkInit!!");
        return false;
    }
    using namespace lsplant;
    bytehook_init(0, true);

    gBinderCanaryT.javaClass = (jclass) env->NewGlobalRef(env->FindClass("com/zipper/develop/binder/canary/BinderCanary"));
    gBinderCanaryT.onReportMethod = JNI_GetStaticMethodID(env, gBinderCanaryT.javaClass, "onReport", "(IILjava/lang/String;)V");

    init_java_monitor_type(env, monitor_config);

    gMonitorConfig.monitor_in_main_thread = get_monitor_in_main_thread(env);
    gMonitorConfig.monitor_large_data = get_config_monitor_large_data(env);
    gMonitorConfig.block_time_threshold_mills = get_config_block_time_ms(env);
    gMonitorConfig.large_data_factor = get_config_large_data_factor(env);
    hasInit = true;
    return hasInit;
}

JNI_STATIC_METHOD(jboolean, monitor)(JNIEnv *env, jclass clazz) {
    return bp_binder_hooker::Hook(gVm);
}

JNI_STATIC_METHOD(jboolean, unmonitored)(JNIEnv *env, jclass clazz) {
    return bp_binder_hooker::UnHook(gVm);
}
