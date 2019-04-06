//
// Created by Jl C on 2019/4/6.
//

#ifndef PJDEMO_PJ_UTILS_H
#define PJDEMO_PJ_UTILS_H

#include "pjlib.h"

#ifdef DEBUG
#define FUNC_ENTER  PJ_LOG(5,("test", "L:%d %s() enters", __LINE__, __FUNCTION__));
#define FUNC_EXIT   PJ_LOG(5,("test", "L:%d %s() exits", __LINE__, __FUNCTION__));
#else
#define FUNC_ENTER
#define FUNC_EXIT
#endif

void app_perror(const char *msg, pj_status_t rc);

#endif //PJDEMO_PJ_UTILS_H
