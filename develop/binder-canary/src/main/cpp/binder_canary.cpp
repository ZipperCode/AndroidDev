
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

static const JNINativeMethod gNativeRegisters[] = {
        {"init", "(Lcom/zipper/develop/binder/canary/MonitorConfig;)Z", (void *) com_zipper_develop_binder_canary_BinderCanary_init}
};

JavaVM *gVm = nullptr;

bool hasInit = false;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    gVm = vm;

    auto canaryClazz = env->FindClass("com/zipper/develop/binder/canary/BinderCanary");
    if (env->RegisterNatives(canaryClazz, gNativeRegisters,
                             sizeof(gNativeRegisters) / sizeof(gNativeRegisters[0])) != JNI_OK) {

        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}


extern "C"
JNIEXPORT jboolean JNICALL
com_zipper_develop_binder_canary_BinderCanary_init(JNIEnv *env, jclass clazz, jobject monitor_config) {
    if (hasInit) {
        LOGW(">> multi call init method，already init!!");
        return false;
    }
    using namespace common;
    if (ArtMethodHandle::get().initialize(gVm)) {
        LOGW(">> init ArtMethodHandle failed");
        return false;
    }

    auto canaryClazz = env->FindClass("com/zipper/develop/binder/canary/BinderCanary");
    auto initNativeMethod = gNativeRegisters[0];
    auto registeredInitNativePtr = initNativeMethod.fnPtr;
    jmethodID initMethodId = env->GetStaticMethodID(canaryClazz, gNativeRegisters[0].name, gNativeRegisters[0].signature);
    auto artMethodPtr = GetArtMethodPtr(env, canaryClazz, initMethodId);
    LOGD("registeredInitNativePtr = %p", registeredInitNativePtr);
    LOGD("artMethodPtr = %p", artMethodPtr);
    auto offset = (size_t) registeredInitNativePtr - (size_t) artMethodPtr;
    LOGD("offset = %ud", offset);
    auto artMethodOffset = ArtMethodHandle::get().getArtMethodOffset();
    auto artMethodPre2 = ArtMethodHandle::get().getArtMethodPtr(env, canaryClazz, initMethodId);
    auto jniInitPtr = ArtMethodHandle::get().getJNIFuncPtr(env, canaryClazz, initMethodId);
    LOGD("artMethodOffset = %d", artMethodOffset);
    LOGD("artMethodPre2 = %p", artMethodPre2);
    LOGD("jniInitPtr = %p", jniInitPtr);

    init_java_monitor_type(env, monitor_config);

    gMonitorConfig.monitor_in_main_thread = get_monitor_in_main_thread(env);
    gMonitorConfig.monitor_large_data = get_config_monitor_large_data(env);
    gMonitorConfig.block_time_threshold_mills = get_config_block_time_ms(env);
    gMonitorConfig.large_data_factor = get_config_large_data_factor(env);

    return hasInit;
}