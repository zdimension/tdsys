/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class MultipleFork */

#ifndef _Included_MultipleFork
#define _Included_MultipleFork
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     MultipleFork
 * Method:    fork
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_MultipleFork_fork
  (JNIEnv *, jclass);

/*
 * Class:     MultipleFork
 * Method:    posix_wait
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_MultipleFork_posix_1wait
  (JNIEnv *, jclass);

/*
 * Class:     MultipleFork
 * Method:    sleep
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_MultipleFork_sleep
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif