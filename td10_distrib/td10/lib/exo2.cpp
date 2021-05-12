//
// Created by Tom on 20/04/2021.
//

#include "exo2.h"
#include <unistd.h>

JNIEXPORT jlong JNICALL Java_exo2_getPid(JNIEnv*, jclass)
{
    return getpid();
}