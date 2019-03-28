#include <jni.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <android/log.h>

#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"NDK",FORMAT,##__VA_ARGS__);

/**
 * 这里使用pthread 获取uuid
 *
 * JNIEnv * 为线程相关的结构体，每一个线程有一个独立的 JNIEnv *
 * 跨线程访问调用需要 通过JVM获取 JNIEnv * 然后操作JNI相关函数
 */

// sdk2.2（不包含2.2）可用 新建线程会自动回调此函数
//JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved){
//    return JNI_VERSION_1_4;
//}

JavaVM* javaVM;
jobject uuid_util_jclass_global;
jmethodID uuidutil_get_method_id;
jint env_version;

extern "C"
JNIEXPORT void JNICALL
Java_com_ubt_posixforandroid_PosixThread_init(JNIEnv *env, jobject instance) {
    // 1. 获取JVM
    env->GetJavaVM(&javaVM);
    env_version=env->GetVersion();
    // 2.获取需要调用UUIDUtil的JNI各个参数（自定义类的jclass必须在调用线程获取，pthread出的线程获取为NULL）
    jclass uuid_util_jclass = env ->FindClass("com/ubt/posixforandroid/UUIDUtil");
    // 子线程调用必须使用GlobalRef全局引用，否则会出现崩溃
    uuid_util_jclass_global = env->NewGlobalRef(uuid_util_jclass);

    // 3.获取get函数
    uuidutil_get_method_id = env->GetStaticMethodID(uuid_util_jclass,"get","()Ljava/lang/String;");
}

void* pthread_runable(void *args){
    JNIEnv *env;
    // 获取JNIEnv
    javaVM->GetEnv((void**)&env,env_version);

    // 绑定线程
    javaVM->AttachCurrentThread(&env,NULL);

    if (env == NULL){
        LOGE("env is null")
    }

    for (int i =0; i < 5; ++i) {

        jobject uuid_jobj = env->CallStaticObjectMethod((jclass)uuid_util_jclass_global,uuidutil_get_method_id);
        const char* uuid_cstr = env->GetStringUTFChars((jstring)uuid_jobj,NULL);

        LOGE("%s",uuid_cstr);
        sleep(1);
    }

    // 解绑线程
    javaVM->DetachCurrentThread();
    pthread_exit((void*)0);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_ubt_posixforandroid_PosixThread_pthread(JNIEnv *env, jobject instance) {
    // 开启线程
    pthread_t pid;
    pthread_create(&pid,NULL,pthread_runable,(void*)pid);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ubt_posixforandroid_PosixThread_destroy(JNIEnv *env, jobject instance) {
    env->DeleteGlobalRef(uuid_util_jclass_global);
    uuidutil_get_method_id=NULL;
}