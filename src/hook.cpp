#include <iostream>

#include <jvmti.h>
#include <jni.h>
#include <jni_md.h>
#include "hook.hpp"

using namespace std;

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
) {
    *new_class_data_len = class_data_len;
    jvmti_env->Allocate(class_data_len, new_class_data);

    unsigned char * my_data = *new_class_data;

    for (int i = 0; i < class_data_len; ++i) {
        my_data[i] = class_data[i];
    }

    cout << "hook()" << name << endl;
    // TODO

    // 放掉内存
}

