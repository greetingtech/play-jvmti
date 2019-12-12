#include <jvmti.h>
#include <jni.h>
#include <jni_md.h>

extern "C" JNIEXPORT jbyteArray JNICALL 
Java_JEncoder_encrypt(
    JNIEnv * _env, 
    jobject _obj,
    jbyteArray _buf
);
