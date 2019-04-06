//
// Created by Jl C on 2019/4/6.
//
#include <android/log.h>

#ifndef PJDEMO_JNI_LOG_H
#define PJDEMO_JNI_LOG_H

//log定义
#define  LOG    "MainActivity" // 这个是自定义的LOG的TAG
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG,__VA_ARGS__) // 定义LOGD类型
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__) // 定义LOGD类型
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG,__VA_ARGS__) // 定义LOGI类型
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG,__VA_ARGS__) // 定义LOGW类型
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG,__VA_ARGS__) // 定义LOGE类型
#define  LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG,__VA_ARGS__) // 定义LOGF类型


#endif //PJDEMO_JNI_LOG_H
