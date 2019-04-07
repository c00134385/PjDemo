//
// Created by Jl C on 2019/4/6.
//

#ifndef PJDEMO_PJTRANS_H
#define PJDEMO_PJTRANS_H

#include <memory>
#include <string>
#include "pj_obj.h"
#include "pj_thread.h"
#include "pj_cb.h"

#define UDP_PORT1   51234
#define ADDRESS1		"127.0.0.1"

enum
{
    READ_FDS,
    WRITE_FDS,
    EXCEPT_FDS
};

class pj_trans : public pj_obj{
public:
    pj_trans(pj_pool_factory *mem);
    ~pj_trans();

    pj_status_t sendto(const void *buf,
                       pj_ssize_t *len,
                       const pj_sockaddr_t *to,
                       int tolen);

    pj_status_t send(const void *buf,
                       pj_ssize_t *len);

    void register_cb(pj_cb *cb);
    pj_bool_t running;
    int do_select();
    int do_select1();

    int do_iobuffer();

public:
    pj_sock_t ss=PJ_INVALID_SOCKET, cs=PJ_INVALID_SOCKET;
    pj_sockaddr_in dstaddr, srcaddr;
    pj_thread *m_thread;
    std::string ipaddr = ADDRESS1;
    pj_uint16_t port = UDP_PORT1;


    /*do_select*/
    pj_sock_t sss[3];

    /*io_queue*/
    pj_ioqueue_t *ioque = NULL;
    pj_ioqueue_key_t *skey = NULL, *ckey = NULL;
    pj_ioqueue_op_key_t read_op, write_op;
    char *send_buf, *recv_buf;
    int bufsize = 1024;

    pj_cb *m_cb = NULL;
};

#endif //PJDEMO_PJTRANS_H
