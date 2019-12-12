
#include "Encrypter.h"

JNIEXPORT jbyteArray JNICALL Java_Encrypter_encrypt
  (JNIEnv *env, jobject obj, jbyteArray bytes) {
          jsize len = env->GetArrayLength(bytes);
    //printf("\nrecv bytes=%d\n", len);
 
    unsigned char* dst = (unsigned char*)env->GetByteArrayElements(bytes, 0);

    dst[0] = 0xaa;
    dst[1] = 0xbb;
    dst[2] = 0xcc;
    dst[3] = 0xdd;

    for (int i = 4; i < len; ++i) {
        dst[i] = ~dst[i];
    } 
 
    env->SetByteArrayRegion(bytes, 0, len, (jbyte *)dst);
    return bytes;
  }