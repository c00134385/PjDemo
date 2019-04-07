//
// Created by Jl C on 2019/4/7.
//

#include "jni_cb.h"
#include "jni_utils.h"
#include <jni.h>
#include <pjlib.h>
#include <pj_utils.h>

extern JavaVM *pj_jni_jvm;

static pj_bool_t attach_jvm(JNIEnv **jni_env)
{
    if ((pj_jni_jvm)->GetEnv((void **)jni_env,
                              JNI_VERSION_1_4) < 0)
    {
        if ((pj_jni_jvm)->AttachCurrentThread(jni_env, NULL) < 0)
        {
            jni_env = NULL;
            return PJ_FALSE;
        }
        return PJ_TRUE;
    }

    return PJ_FALSE;
}

#define detach_jvm(attached) \
    if (attached) \
        (pj_jni_jvm)->DetachCurrentThread();


static jni_cb * m_instance = nullptr;
static std::mutex m_mutex;

jni_cb* jni_cb::create(JNIEnv* env, jobject jcb) {
    std::unique_lock<std::mutex> lck (m_mutex);
    if (m_instance)
    {
        delete m_instance;
    }
    m_instance = new jni_cb(env, jcb);
    return m_instance;
}

void jni_cb::destroy() {
    if(m_instance) {
        delete m_instance;
        m_instance = NULL;
    }
}

jni_cb::jni_cb(JNIEnv *env, jobject jcb) : j_cb(env->NewGlobalRef(jcb)) {
    jclass jclz = NULL;
    jmethodID jmeth = NULL;

    jclz = env->GetObjectClass(jcb);
    if(!jclz) {
        PJ_LOG(1,("", "get class failed."));
        goto on_error;
    }

    jmeth = env->GetMethodID(jclz, "hashCode", "()I");
    if(!jmeth) {
        PJ_LOG(1,("", "get method failed."));
        goto on_error;
    }

    hashCode = env->CallIntMethod(j_cb, jmeth);
    PJ_LOG(1,("", "hashCode:%d", hashCode));
on_error:
    if(jclz) {
        env->DeleteLocalRef(jclz);
    }
    return;
}

jni_cb::~jni_cb() {
    FUNC_ENTER
    PJ_LOG(1,("", "hashCode:%d", hashCode));
    FUNC_EXIT
}

int jni_cb::on_event(int event, const char *param1, const char *param2) {
    JNIEnv *env = 0;
    jclass jclz = NULL;
    jmethodID jmeth = NULL;
    jstring param1_;
    jint rc;

    pj_bool_t attached = attach_jvm(&env);
    if (!env)
        goto on_error;

    jclz = env->GetObjectClass(j_cb);
    if(!jclz) {
        PJ_LOG(1,("", "get class failed."));
        goto on_error;
    }

    jmeth = env->GetMethodID(jclz, "onEvent", "(ILjava/lang/String;)I");
    if(!jmeth) {
        PJ_LOG(1,("", "get method failed."));
        goto on_error;
    }

    param1_ = string2jstring(env, param1);
    rc = env->CallIntMethod(j_cb, jmeth, event, param1_);
    PJ_LOG(1,("", "rc:%d", rc));

on_error:
    if(param1_) {
        env->DeleteLocalRef(param1_);
    }
    if(jclz) {
        env->DeleteLocalRef(jclz);
    }
    detach_jvm(attached);
    return 0;
}