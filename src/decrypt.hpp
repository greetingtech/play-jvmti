#include <jvmti.h>
#include <jni.h>
#include <jni_md.h>

void decrypt(
    jvmtiEnv * jvmti_env,
    const unsigned char * class_data,
    jint class_data_len,
    jint * new_class_data_len,
    unsigned char ** new_class_data
);