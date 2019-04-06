//
// Created by Jl C on 2019/4/6.
//

#include "pjutils.h"
#include <pjlib.h>
#include "jni_log.h"

void app_perror(const char *msg, pj_status_t rc)
{
    char errbuf[PJ_ERR_MSG_SIZE];

    PJ_CHECK_STACK();

    pj_strerror(rc, errbuf, sizeof(errbuf));
    PJ_LOG(3,("test", "%s: [pj_status_t=%d] %s", msg, rc, errbuf));
}

void app_log_func(int level, const char *data, int len) {
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