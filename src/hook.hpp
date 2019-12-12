#include <jvmti.h>
#include <jni.h>
#include <jni_md.h>

void JNICALL DecryptClassFileLoadHook (
    jvmtiEnv * jvmti_env,
    JNIEnv * jni_env,
    jclass class_being_redefined,
    jobject loader,
    const char * name,
    jobject protection_domain,
    jint class_data_len,
    const unsigned char * class_data,
    jint * new_class_data_len,
    unsigned char ** new_class_data
);