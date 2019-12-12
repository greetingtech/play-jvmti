#include <iostream>

#include <jvmti.h>
#include <jni.h>
#include <jni_md.h>

using namespace std;

void JNICALL MyClassFileLoadHook (
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

JNIEXPORT jint JNICALL
Agent_OnLoad(
    JavaVM *vm,
    char *options,
    void *reserved
) {
    jvmtiEnv *jvmti;

    jint ret = vm->GetEnv((void **)&jvmti, JVMTI_VERSION);
    if (JNI_OK != ret) {
        cout << "ERROR GetEnv" << endl;
        return ret;
    }
    jvmtiCapabilities capabilities;
    (void)memset(&capabilities, 0, sizeof(capabilities));

    capabilities.can_generate_all_class_hook_events   = 1;
    capabilities.can_tag_objects                      = 1;
    capabilities.can_generate_object_free_events      = 1;
    capabilities.can_get_source_file_name             = 1;
    capabilities.can_get_line_numbers                 = 1;
    capabilities.can_generate_vm_object_alloc_events  = 1;
 
    jvmtiError error = jvmti->AddCapabilities(&capabilities);
    if (JVMTI_ERROR_NONE != error) {
        cout << "ERROR AddCapabilities";
        return ret;
    }

    jvmtiEventCallbacks callbacks;
    (void)memset(&callbacks, 0, sizeof(callbacks));

    callbacks.ClassFileLoadHook = MyClassFileLoadHook;
    error = jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));

    if (JVMTI_ERROR_NONE != error) {
        cout << "ERROR SetEventCallbacks";
        return ret;
    }

    error = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
    if (JVMTI_ERROR_NONE != error) {
        cout << "ERROR SetEventNotificationMode";
        return ret;
    }

    cout << "JNI_OK";

    return JNI_OK;
}
