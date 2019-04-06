//
// Created by Jl C on 2019/4/6.
//

#ifndef PJDEMO_PJ_OBJ_H
#define PJDEMO_PJ_OBJ_H

#include "pjlib.h"

class pj_obj {
public:
    pj_obj(pj_pool_factory *mem);
    ~pj_obj();

protected:
    pj_pool_t *pool;
};

#endif //PJDEMO_PJ_OBJ_H
