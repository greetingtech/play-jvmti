#include <iostream>

#include "hook.hpp"

using namespace std;

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
    memset(&capabilities, 0, sizeof(capabilities));
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
    memset(&callbacks, 0, sizeof(callbacks));

    callbacks.ClassFileLoadHook = DecryptClassFileLoadHook;
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

    cout << "Agent_OnLoad Finished" << endl;

    return JNI_OK;
}
