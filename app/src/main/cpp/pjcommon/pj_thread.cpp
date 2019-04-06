//
// Created by Jl C on 2019/4/6.
//

#include "pj_thread.h"
#include "pj_utils.h"

pj_thread::pj_thread(pj_pool_factory *mem,  pj_thread_proc* func, void* param) : pj_obj(mem), proc_fun(func) {
    pj_status_t rc;
    rc = pj_thread_create(pool, "thread", (pj_thread_proc*)proc_fun,
                          param,
                          PJ_THREAD_DEFAULT_STACK_SIZE,
                          0,
                          &thread);
}

pj_thread::~pj_thread() {
    pj_status_t rc;
    rc = pj_thread_join(thread);
    if (rc != PJ_SUCCESS) {
        app_perror("...ERROR in pj_thread_join()", rc);
    }
    rc = pj_thread_destroy(thread);
    if (rc != PJ_SUCCESS) {
        app_perror("...ERROR in pj_thread_destroy()", rc);
    }
}