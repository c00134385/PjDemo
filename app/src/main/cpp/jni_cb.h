//
// Created by Jl C on 2019/4/7.
//

#ifndef PJDEMO_JNI_CB_H
#define PJDEMO_JNI_CB_H

#include "pj_cb.h"
#include <mutex>
#include <jni.h>

class jni_cb : public pj_cb{
public:
    static jni_cb* create(JNIEnv* env, jobject jobj);
    static void destroy();

private:
    jni_cb(JNIEnv* env, jobject jobj);
    ~jni_cb();

    virtual int on_event(int event, const char *param1, const char *param2) override ;

private:
//    static jni_cb * m_instance;
//    static std::mutex m_mutex;
    jobject j_cb;
    jint hashCode = 0;
};

#endif //PJDEMO_JNI_CB_H
