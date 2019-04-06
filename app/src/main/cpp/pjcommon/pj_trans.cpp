//
// Created by Jl C on 2019/4/6.
//

#include "pj_trans.h"
#include "pj_utils.h"

#define UDP_PORT    51234
#define ADDRESS		"127.0.0.1"

int trans_proc(void* params) {
    FUNC_ENTER
    pj_trans* trans = (pj_trans*)params;
    int count = 0;
    while (trans->running) {
        PJ_LOG(1, ("", "count:%d", count++));
        pj_thread_sleep(1000);
    }
    FUNC_EXIT
    return 0;
}

pj_trans::pj_trans(pj_pool_factory *mem) : pj_obj(mem) {

    pj_status_t rc;
    rc = pj_sock_socket(pj_AF_INET(), pj_SOCK_DGRAM(), 0, &ss);
    if (rc != 0) {
        app_perror("...error: unable to create ss socket", rc);
    }

    rc = pj_sock_socket(pj_AF_INET(), pj_SOCK_DGRAM(), 0, &cs);
    if (rc != 0) {
        app_perror("...error: unable to create cs socket", rc);
    }

    pj_str_t s;
    /* Bind server socket. */
    pj_bzero(&dstaddr, sizeof(dstaddr));
    dstaddr.sin_family = pj_AF_INET();
    dstaddr.sin_port = pj_htons(UDP_PORT);
    dstaddr.sin_addr = pj_inet_addr(pj_cstr(&s, ADDRESS));

    if ((rc=pj_sock_bind(ss, &dstaddr, sizeof(dstaddr))) != 0) {
        app_perror("...bind error udp:", rc);
    }

    /* Bind client socket. */
    pj_bzero(&srcaddr, sizeof(srcaddr));
    srcaddr.sin_family = pj_AF_INET();
    srcaddr.sin_port = pj_htons(UDP_PORT-1);
    srcaddr.sin_addr = pj_inet_addr(pj_cstr(&s, ADDRESS));

    if ((rc=pj_sock_bind(cs, &srcaddr, sizeof(srcaddr))) != 0) {
        app_perror("...bind error", rc);
    }

    running = PJ_TRUE;
    m_thread = new pj_thread(mem, trans_proc, this);

}

pj_trans::~pj_trans() {
    pj_status_t rc;
    FUNC_ENTER
    running = PJ_FALSE;
    delete m_thread;

    if (cs != PJ_INVALID_SOCKET) {
        rc = pj_sock_close(cs);
        if (rc != PJ_SUCCESS) {
            app_perror("...error in closing cs socket", rc);
        }
    }
    if (ss != PJ_INVALID_SOCKET) {
        rc = pj_sock_close(ss);
        if (rc != PJ_SUCCESS) {
            app_perror("...error in closing ss socket", rc);
        }
    }
}