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
        int size = sizeof(args);
        jobjectArray arr = static_cast<jobjectArray>(args);
        jobjectArray *arr2 = (jobjectArray*) args;
        int size1 = sizeof(arr);
        int size2 = sizeof(arr2);
        int len = env->GetArrayLength(arr);

        jclass vmClass = env->FindClass("dalvik/system/VMRuntime");
        jmethodID runtimeMethod = env->GetStaticMethodID(vmClass, "getRuntime", "()Ldalvik/system/VMRuntime;");
        jobject runtime = env->CallStaticObjectMethod(vmClass, runtimeMethod);
        jmethodID setHiddenApiExemptionsMethodId = env->GetMethodID(vmClass, "setHiddenApiExemptions", "([Ljava/lang/String;)V");

        jclass stringClass = env->FindClass("java/lang/String");
        jobjectArray stringArray = env->NewObjectArray(1, stringClass, env->NewStringUTF("L"));

        env->CallVoidMethod(runtime, setHiddenApiExemptionsMethodId, stringArray);
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