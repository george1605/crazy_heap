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

        if(ptr != nullptr && length > 0) {
            munmap(ptr, length);
        }
    }
}