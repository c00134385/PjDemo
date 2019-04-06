//
// Created by Jl C on 2019/4/6.
//
#include "jni_log.h"

void jni_log_func(int level, const char *data, int len) {
    switch (level) {
        case 1:
            LOGF("%s", data);
            break;
        case 2:
            LOGE("%s", data);
            break;
        case 3:
            LOGW("%s", data);
            break;
        case 4:
            LOGI("%s", data);
            break;
        case 5:
            LOGD("%s", data);
            break;
        case 6:
        default:
            LOGV("%s", data);
    }
}