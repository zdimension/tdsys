//
// Created by Tom on 20/04/2021.
//

#include <jni.h>
#include <cstdlib>
#include <cstring>

#define MOT_DE_PASSE_1 420420365
#define MOT_DE_PASSE_2 "polytech"

int getHash(const char* ptr)
{
    int res = 0;
    while (*ptr && *(ptr + 1) && *(ptr + 2) && *(ptr + 3))
    {
        res ^= *(int*)ptr;
        ptr++;
    }
    return res;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;

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
    {
        abort();
    }

    char* name = argv[1];
    jclass jcls = jniEnv->FindClass(name);
    if (!jcls)
    {
        fprintf(stderr, "Error: Could not find or load main class %s\n"
                        "Caused by: java.lang.ClassNotFoundException: %s\n", name, name);
        return 1;
    }

    jfieldID pwField = jniEnv->GetStaticFieldID(jcls, "MOT_DE_PASSE", "Ljava/lang/String;");
    const char* pw;
    if (!pwField
    || !(pw = jniEnv->GetStringUTFChars((jstring)jniEnv->GetStaticObjectField(jcls, pwField), nullptr))
    || getHash(pw) != MOT_DE_PASSE_1)
    {
        fprintf(stderr, "Error: Invalid bytecode\n");
        return 1;
    }
    jniEnv->SetStaticObjectField(jcls, pwField, jniEnv->NewStringUTF(MOT_DE_PASSE_2));
    jmethodID methodId = jniEnv->GetStaticMethodID(jcls, "main", "([Ljava/lang/String;)V");
    if (!methodId)
    {
        fprintf(stderr, "Error: Main method not found in class %s, please define the main method as:\n"
                        "   public static void main(String[] args)"
                        "or a JavaFX application class must extend javafx.application.Application\n",
                name);
        return 1;
    }

    jniEnv->CallStaticVoidMethod(jcls, methodId, NULL);
}