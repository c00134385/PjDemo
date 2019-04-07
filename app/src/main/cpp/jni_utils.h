//
// Created by Jl C on 2019/4/7.
//

#ifndef PJDEMO_JNI_UTILS_H
#define PJDEMO_JNI_UTILS_H

#include <jni.h>

char* jstring2string(JNIEnv* env, jstring jstr);

jstring string2jstring(JNIEnv* env, const char* pat);

#endif //PJDEMO_JNI_UTILS_H
