// The JNI C layer
// These functions are exported to Java via a .so file

#include "Mem.h"
#include <jni.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

// just for test, use this to see if it works
// if not check the compile.sh file and paths
extern "C" {
    JNIEXPORT jobject JNICALL
    Java_Mem_alloc(JNIEnv *env, jobject obj, jlong pages, jint flags) {
        size_t pagesz = getpagesize();
        size_t length = pages * pagesz;

        void* ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(ptr == MAP_FAILED)
            return NULL;

        return env->NewDirectByteBuffer(ptr, length);
    }

    JNIEXPORT void JNICALL
    Java_Mem_free(JNIEnv *env, jobject obj, jobject buffer) {
        if(buffer == NULL) return;
        void* ptr = env->GetDirectBufferAddress(buffer);
        jlong length = env->GetDirectBufferCapacity(buffer);

        if(ptr != NULL && length > 0) {
            munmap(ptr, length);
        }
    }

    static void* initial_brk = NULL;

    JNIEXPORT jobject JNICALL
    Java_Mem_sbrk(JNIEnv* env, jobject obj, jlong size) {
        if(initial_brk == NULL)
            initial_brk = sbrk(0);

        void* ptr = sbrk(size);
        if (ptr == NULL || size <= 0)
            return NULL;
        return env->NewDirectByteBuffer(ptr, size);
    }

    JNIEXPORT void JNICALL
    Java_Mem_brkclean(JNIEnv* env, jobject obj) {
        jclass exceptionClass = env->FindClass("java/lang/IllegalStateException");
        if(initial_brk == NULL)
        {
            env->ThrowNew(exceptionClass, "initial_brk is null.");
        } else {
            brk(initial_brk);
        }
    }

}