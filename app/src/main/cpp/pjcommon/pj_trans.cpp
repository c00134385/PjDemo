//
// Created by Jl C on 2019/4/6.
//

#include "pj_trans.h"
#include "pj_utils.h"


int trans_proc(void* params) {
    FUNC_ENTER
    pj_trans* trans = (pj_trans*)params;
    int count = 0;
    char recvdata[512+4];
    pj_memset(recvdata, 0, sizeof(recvdata));
    pj_ssize_t received = sizeof(received);
    while (trans->running) {
        PJ_LOG(1, ("", "count:%d", count++));
        pj_status_t rc;
        pj_sockaddr_in addr;
        int srclen = sizeof(addr);

        pj_bzero(&addr, sizeof(addr));
#if 1
        rc = pj_sock_recvfrom(trans->ss, recvdata, &received, 0, &addr, &srclen);
        if (rc != PJ_SUCCESS) {
            app_perror("...recvfrom error", rc);
            rc = -150;
        } else {
            PJ_LOG(1, ("", "received:%d data:%s addr:%x port:%d", received, recvdata, addr.sin_addr.s_addr, addr.sin_port));
        }
#else
        rc = pj_sock_recv(ss, recvdata, &received, 0);
	    if (rc != PJ_SUCCESS) {
	        app_perror("...recv error", rc);
	        rc = -155; goto on_error;
	    } else {
	        PJ_LOG(1, ("", "received:%d data:%s", received, recvdata));
	    }
#endif

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
    dstaddr.sin_port = pj_htons(port);
    dstaddr.sin_addr = pj_inet_addr(pj_cstr(&s, ipaddr.c_str()));

    if ((rc=pj_sock_bind(ss, &dstaddr, sizeof(dstaddr))) != 0) {
        app_perror("...bind error udp:", rc);
    }

    /* Bind client socket. */
    pj_bzero(&srcaddr, sizeof(srcaddr));
    srcaddr.sin_family = pj_AF_INET();
    srcaddr.sin_port = pj_htons(port-1);
    srcaddr.sin_addr = pj_inet_addr(pj_cstr(&s, ipaddr.c_str()));

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

pj_status_t pj_trans::sendto(const void *buf,
                   pj_ssize_t *len,
                   const pj_sockaddr_t *to,
                   int tolen) {
    pj_status_t rc;
    pj_ssize_t sent = *len;
    PJ_LOG(3, ("test", "sendto data:%s len:%d", buf, sent));
    rc = pj_sock_sendto(cs, buf, &sent, 0, to, tolen);
    if (rc != PJ_SUCCESS || sent != *len) {
        app_perror("...sendto error", rc);
        rc = -140;
    }

    return rc;
}

pj_status_t pj_trans::send(const void *buf,
                 pj_ssize_t *len) {
    pj_status_t rc;
    pj_ssize_t sent = *len;
    PJ_LOG(3, ("test", "send data:%s len:%d", buf, len));
    rc = pj_sock_send(cs, buf, &sent, 0);
    if (rc != PJ_SUCCESS || sent != *len) {
        app_perror("...send error", rc);
        rc = -140;
    }

    return rc;
}