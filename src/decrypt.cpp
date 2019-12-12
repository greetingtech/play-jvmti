#include <iostream>

#include "decrypt.hpp"

using namespace std;

const unsigned char java_magic[] = {0xca, 0xfe, 0xba, 0xbe};

const unsigned char encry_magic[] = {0xaa, 0xbb, 0xcc, 0xdd}; 

bool isEncryptedClass(const unsigned char * class_data, jint class_data_len) {
    if (class_data_len < 4) {
        return false;
    }
    for (int i = 0; i < 4; ++i) {
        if (class_data[i] != encry_magic[i]) {
            return false;
        }
    }
    return true;
}

void decrypt(
    jvmtiEnv * jvmti_env,
    const unsigned char * class_data,
    jint class_data_len,
    jint * new_class_data_len,
    unsigned char ** new_class_data
) {
    
    bool is = isEncryptedClass(class_data, class_data_len);

    if (!is) {
        return;
    }
    
    jvmtiError error = jvmti_env->Allocate(class_data_len, new_class_data);
    
    if (error != JVMTI_ERROR_NONE) {
        cout << "jvmti_env->Allocate error " << error << endl;
    }

    if (class_data_len < 4) {
        return;
    }

    *new_class_data_len = class_data_len;

    unsigned char * new_class_data_address = (*new_class_data);

    for (int i = 0; i < 4; ++i) {
        (*new_class_data)[i] = java_magic[i];
    }
    int len = *new_class_data_len;

    for (int i = 4; i < len; ++i) {
        (*new_class_data)[i] = ~(class_data[i]);
    }

}