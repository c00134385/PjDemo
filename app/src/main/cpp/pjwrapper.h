//
// Created by Jl C on 2019/4/6.
//

#ifndef PJDEMO_PJWRAPPER_H
#define PJDEMO_PJWRAPPER_H

#include "pj_cb.h"

int pjwrapper_init();

int pjwrapper_uninit();

int pjwrapper_start(pj_cb *cb);

int pjwrapper_stop();

int pjwrapper_action();

#endif //PJDEMO_PJWRAPPER_H
