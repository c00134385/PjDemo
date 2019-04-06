//
// Created by Jl C on 2019/4/6.
//

#ifndef PJDEMO_PJTRANS_H
#define PJDEMO_PJTRANS_H

#include <memory>
#include "pj_obj.h"
#include "pj_thread.h"

class pj_trans : public pj_obj{
public:
    pj_trans(pj_pool_factory *mem);
    ~pj_trans();

    pj_bool_t running;
private:
    pj_sock_t ss=PJ_INVALID_SOCKET, cs=PJ_INVALID_SOCKET;
    pj_sockaddr_in dstaddr, srcaddr;
    pj_thread *m_thread;
};

#endif //PJDEMO_PJTRANS_H
