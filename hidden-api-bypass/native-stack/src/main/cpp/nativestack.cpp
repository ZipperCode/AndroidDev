#include <jni.h>
#include <string>
#include <pthread.h>

static JavaVM *sVm;

void *run(void * args) {
    JNIEnv *env = nullptr;
    int ret = -1;
    if (sVm->AttachCurrentThread(&env, nullptr) == 0) {
        if (env == nullptr) {
            return (void *)ret;
        }
        auto arr = static_cast<jobjectArray>(args);
        int len = env->GetArrayLength(arr);
        jclass stringClass = env->FindClass("java/lang/String");
        jobjectArray newArray = env->NewObjectArray(len, stringClass, nullptr);
        for (int i = 0; i < len; i++) {
            auto item = env->GetObjectArrayElement(arr, i);
            env->SetObjectArrayElement(newArray, i, item);
        }

        jclass vmClass = env->FindClass("dalvik/system/VMRuntime");
        jmethodID runtimeMethod = env->GetStaticMethodID(vmClass, "getRuntime", "()Ldalvik/system/VMRuntime;");
        jobject runtime = env->CallStaticObjectMethod(vmClass, runtimeMethod);
        jmethodID setHiddenApiExemptionsMethodId = env->GetMethodID(vmClass, "setHiddenApiExemptions", "([Ljava/lang/String;)V");
        env->CallVoidMethod(runtime, setHiddenApiExemptionsMethodId, newArray);
        ret = 0;
    }
    return (void *)ret;
}


jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *mainEnv;
    jint res = -1;
    sVm = vm;
    if (vm->GetEnv(reinterpret_cast<void **>(&mainEnv), JNI_VERSION_1_6) != JNI_OK) {
        return res;
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zipper_hiddenapibypass_nativestack_NativeStackReflection_exempt(JNIEnv *env, jclass clazz, jobjectArray args) {
    pthread_t pid;
    pthread_create(&pid, nullptr, run, args);
    jint threadRes = -1;
    int ret = pthread_join(pid, (void **)&threadRes);
    if (ret == -1) {
        return JNI_FALSE;
    }
    if (threadRes == -1) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}