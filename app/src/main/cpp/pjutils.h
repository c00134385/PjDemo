//
// Created by Jl C on 2019/4/6.
//

#ifndef PJDEMO_PJUTILS_H
#define PJDEMO_PJUTILS_H

#include <pjlib.h>

void app_perror(const char *msg, pj_status_t rc);
void app_log_func(int level, const char *data, int len);


#ifdef DEBUG
#define FUNC_ENTER  PJ_LOG(5,("test", "L:%d %s() enters", __LINE__, __FUNCTION__));
#define FUNC_EXIT   PJ_LOG(5,("test", "L:%d %s() exits", __LINE__, __FUNCTION__));
#else
#define FUNC_ENTER
#define FUNC_EXIT
#endif

#endif //PJDEMO_PJUTILS_H
