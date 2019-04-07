//
// Created by Jl C on 2019/4/6.
//

#include "pj_utils.h"

void app_perror(const char *msg, pj_status_t rc)
{
    char errbuf[PJ_ERR_MSG_SIZE];

    PJ_CHECK_STACK();

    pj_strerror(rc, errbuf, sizeof(errbuf));
    PJ_LOG(1,("test", "%s: [pj_status_t=%d] %s", msg, rc, errbuf));
}