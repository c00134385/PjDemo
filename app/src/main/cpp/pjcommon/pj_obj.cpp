//
// Created by Jl C on 2019/4/6.
//

#include "pj_obj.h"

pj_obj::pj_obj(pj_pool_factory *mem) {
    pool = pj_pool_create(mem, NULL, 4000, 4000, NULL);
    if (!pool){
        PJ_LOG(1, ("pj_obj", "construct failed."));
    }
}

pj_obj::~pj_obj() {
    if(pool) {
        pj_pool_release(pool);
    }
}