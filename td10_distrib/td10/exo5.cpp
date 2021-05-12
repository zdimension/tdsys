//
// Created by Tom on 20/04/2021.
//

#include <jni.h>
#include <unistd.h>

int main()
{
    JavaVMOption jvmopt[1];
    jvmopt[0].optionString = "-Djava.class.path=.";

    JavaVMInitArgs vmArgs = {0};
    vmArgs.version = JNI_VERSION_10;
    vmArgs.nOptions = 1;
    vmArgs.options = jvmopt;
    vmArgs.ignoreUnrecognized = JNI_TRUE;

    JavaVM* javaVM;
    JNIEnv* jniEnv;
    if (JNI_CreateJavaVM(&javaVM, (void**) &jniEnv, &vmArgs) == JNI_ERR)
        fprintf(stderr, "erreur jvm\n");

    jclass jcls = jniEnv->FindClass("exo1");
    if (!jcls)
        fprintf(stderr, "erreur classe\n");
    jmethodID methodId = jniEnv->GetStaticMethodID(jcls, "main", "([Ljava/lang/String;)V");
    if (!methodId)
        fprintf(stderr, "erreur méthode\n");
    jniEnv->CallStaticVoidMethod(jcls, methodId, NULL);

    printf("%d\n", execlp("java", "java", "exo1"));
}