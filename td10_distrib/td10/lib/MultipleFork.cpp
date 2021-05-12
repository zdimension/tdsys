//
// Created by Tom on 20/04/2021.
//

#include "MultipleFork.h"
#include <unistd.h>
#include <sys/wait.h>

JNIEXPORT jlong JNICALL Java_MultipleFork_fork(JNIEnv*, jclass)
{
    return fork();
}

JNIEXPORT jlong JNICALL Java_MultipleFork_posix_1wait(JNIEnv*, jclass)
{
    return wait(nullptr);
}

JNIEXPORT void JNICALL Java_MultipleFork_sleep(JNIEnv*, jclass, jint i)
{
    sleep(i);
}