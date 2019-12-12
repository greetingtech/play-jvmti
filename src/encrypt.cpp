#include <iostream>

#include "encrypt.hpp"
 
extern "C" JNIEXPORT jbyteArray JNICALL 
Java_JEncoder_encrypt(
    JNIEnv * _env, 
    jobject _obj,
    jbyteArray _buf
)
{
    jsize len =_env->GetArrayLength(_buf);
    //printf("\nrecv bytes=%d\n", len);
 
    unsigned char* dst = (unsigned char*)_env->GetByteArrayElements(_buf, 0);

    dst[0] = 0xaa;
    dst[1] = 0xbb;
    dst[2] = 0xcc;
    dst[3] = 0xdd;

    for (int i = 4; i < len; ++i) {
        dst[i] = ~dst[i];
    } 
 
    _env->SetByteArrayRegion(_buf, 0, len, (jbyte *)dst);
    return _buf;
}