//
// Created by Jl C on 2019/4/7.
//

#ifndef PJDEMO_PJ_CB_H
#define PJDEMO_PJ_CB_H

class pj_cb {
public:
    virtual int on_event(int event, const char *param1, const char *param2) = 0;
};
#endif //PJDEMO_PJ_CB_H
