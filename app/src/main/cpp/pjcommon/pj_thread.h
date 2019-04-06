//
// Created by Jl C on 2019/4/6.
//

#ifndef PJDEMO_PJ_THREAD_H
#define PJDEMO_PJ_THREAD_H

#include "pj_obj.h"

class pj_thread : public pj_obj{
public:
    pj_thread(pj_pool_factory *mem, pj_thread_proc* func, void* param);
    ~pj_thread();

private:
    pj_thread_t *thread;
    pj_thread_proc *proc_fun;
};

#endif //PJDEMO_PJ_THREAD_H
