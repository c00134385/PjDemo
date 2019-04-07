//
// Created by Jl C on 2019/4/6.
//

#include <sys/ioctl.h>
#include "pj_trans.h"
#include "pj_utils.h"

#define BASIC_SOCKET  0
#define SELECT_SOCKET 1
#define IOBUFF_SOCKET 0

#define THIS_FILE	    "test_udp"

static pj_ssize_t            callback_read_size,
        callback_write_size,
        callback_accept_status,
        callback_connect_status;
static pj_ioqueue_key_t     *callback_read_key,
        *callback_write_key,
        *callback_accept_key,
        *callback_connect_key;
static pj_ioqueue_op_key_t  *callback_read_op,
        *callback_write_op,
        *callback_accept_op;

static void on_ioqueue_read(pj_ioqueue_key_t *key,
                            pj_ioqueue_op_key_t *op_key,
                            pj_ssize_t bytes_read)
{
    callback_read_key = key;
    callback_read_op = op_key;
    callback_read_size = bytes_read;
    PJ_LOG(1, ("", "callback_read_size:%d", callback_read_size));
//    TRACE__((THIS_FILE, "     callback_read_key = %p, bytes=%d",
//            key, bytes_read));
}

static void on_ioqueue_write(pj_ioqueue_key_t *key,
                             pj_ioqueue_op_key_t *op_key,
                             pj_ssize_t bytes_written)
{
    callback_write_key = key;
    callback_write_op = op_key;
    callback_write_size = bytes_written;
    PJ_LOG(1, ("", "callback_write_size:%d", callback_write_size));
}

static void on_ioqueue_accept(pj_ioqueue_key_t *key,
                              pj_ioqueue_op_key_t *op_key,
                              pj_sock_t sock, int status)
{
    PJ_UNUSED_ARG(sock);
    callback_accept_key = key;
    callback_accept_op = op_key;
    callback_accept_status = status;
}

static void on_ioqueue_connect(pj_ioqueue_key_t *key, int status)
{
    callback_connect_key = key;
    callback_connect_status = status;
}

static pj_ioqueue_callback test_cb =
        {
                &on_ioqueue_read,
                &on_ioqueue_write,
                &on_ioqueue_accept,
                &on_ioqueue_connect,
        };


int trans_proc(void* params) {
    FUNC_ENTER
    pj_trans* trans = (pj_trans*)params;
    int count = 0;
    char recvdata[512+4];
    pj_memset(recvdata, 0, sizeof(recvdata));
    pj_ssize_t received = sizeof(received);
    while (trans->running) {
        pj_status_t rc;
        pj_sockaddr_in addr;
        int srclen = sizeof(addr);

        pj_bzero(&addr, sizeof(addr));
#if BASIC_SOCKET
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
#elif SELECT_SOCKET
        rc = trans->do_select1();
	    if(rc <= 0) {
            continue;
	    }
#elif IOBUFF_SOCKET
	    // iobuff test
        trans->do_iobuffer();
#endif
        if(trans->m_cb) {
	        trans->m_cb->on_event(1, "received", "data");
	    }
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

    for(int i = 0; i < 3; i++) {
        rc = pj_sock_socket(pj_AF_INET(), pj_SOCK_DGRAM(), 0, &sss[i]);
        if (rc != 0) {
            app_perror("...error: unable to create cs socket", rc);
        }
        /* Bind server socket. */
        pj_bzero(&dstaddr, sizeof(dstaddr));
        dstaddr.sin_family = pj_AF_INET();
        dstaddr.sin_port = pj_htons(port + 1 + i);
        dstaddr.sin_addr = pj_inet_addr(pj_cstr(&s, ipaddr.c_str()));

        PJ_LOG(1, ("","port:%d ipaddr:%s", port + 1 + i, ipaddr.c_str()));
        if ((rc=pj_sock_bind(sss[i], &dstaddr, sizeof(dstaddr))) != 0) {
            app_perror("...bind error udp:", rc);
        }
    }

    send_buf = (char*)pj_pool_alloc(pool, bufsize);
    recv_buf = (char*)pj_pool_alloc(pool, bufsize);

    PJ_LOG(1, ("", "PJ_IOQUEUE_MAX_HANDLES:%d", PJ_IOQUEUE_MAX_HANDLES));
    rc = pj_ioqueue_create(pool, PJ_IOQUEUE_MAX_HANDLES, &ioque);
    if (rc != PJ_SUCCESS) {
        app_perror("...pj_ioqueue_create failed:", rc);
    }

    rc = pj_ioqueue_set_default_concurrency(ioque, PJ_TRUE);
    if (rc != PJ_SUCCESS) {
        app_perror("...pj_ioqueue_set_default_concurrency failed:", rc);
    }

    rc = pj_ioqueue_register_sock(pool, ioque, ss, NULL,
                                  &test_cb, &skey);
    if (rc != PJ_SUCCESS) {
        app_perror("...error(10): ioqueue_register error", rc);
    }
    rc = pj_ioqueue_register_sock( pool, ioque, cs, NULL,
                                   &test_cb, &ckey);
    if (rc != PJ_SUCCESS) {
        app_perror("...error(11): ioqueue_register error", rc);
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

void pj_trans::register_cb(pj_cb *cb) {
    this->m_cb = cb;
}

int pj_trans::do_select() {
    pj_status_t rc;
    pj_fd_set_t fds[3];
    pj_time_val timeout;
    int i, n;

    int setcount[3];

    for (i=0; i<3; ++i) {
        PJ_FD_ZERO(&fds[i]);
        setcount[i] = 0;
    }

    PJ_FD_SET(ss, &fds[0]);
    PJ_FD_SET(sss[0], &fds[0]);
    PJ_FD_SET(sss[1], &fds[0]);
    PJ_FD_SET(sss[2], &fds[0]);

    timeout.sec = 2;
    timeout.msec = 0;

    rc = pj_sock_select(FD_SETSIZE, &fds[0], &fds[1], &fds[2], NULL);
    if (rc < 0) {
        app_perror("...send error", pj_get_netos_error());
        return rc;
    } else if(rc == 0) {
        app_perror("...timeout....", pj_get_netos_error());
        return rc;
    } else {
        if (PJ_FD_ISSET(ss, &fds[0])) {
            int nread;
            ioctl(ss, FIONREAD, &nread);//取得数据量交给nread
            char recvdata[512+4];
            pj_memset(recvdata, 0, sizeof(recvdata));
            pj_ssize_t received = sizeof(received);
            rc = pj_sock_recv(ss, recvdata, &received, 0);
            if (rc != PJ_SUCCESS) {
                app_perror("...recv error", rc);
                rc = -155;
            } else {
                PJ_LOG(1, ("", "received:%d data:%s  nread:%d", received, recvdata, nread));
            }
        }

//        for (i = 0; i < 3; ++i) {
//            if (PJ_FD_ISSET(sss[0], &fds[i]))
//                setcount[i]++;
//            if (PJ_FD_ISSET(sss[1], &fds[i]))
//                setcount[i]++;
//            if (PJ_FD_ISSET(sss[2], &fds[i]))
//                setcount[i]++;
//        }

        PJ_LOG(1,("", "setcount:%d-%d-%d", setcount[READ_FDS], setcount[WRITE_FDS], setcount[EXCEPT_FDS]));

        return rc;
    }

}

int pj_trans::do_select1() {
    pj_status_t rc;

    int result;
    fd_set readfds, testfds;
    FD_ZERO(&readfds);
    FD_SET(ss, &readfds);//将服务器端socket加入到集合中
    FD_SET(sss[0], &readfds);//将服务器端socket加入到集合中
    FD_SET(sss[1], &readfds);//将服务器端socket加入到集合中
    FD_SET(sss[2], &readfds);//将服务器端socket加入到集合中



    while (1) {
        char ch;
        int fd;
        int nread;
        testfds = readfds;//将需要监视的描述符集copy到select查询队列中，select会对其修改，所以一定要分开使用变量

        PJ_LOG(1, ("test", "server waiting %d-%d-%d-%d", ss, sss[0], sss[1], sss[2]));
        /*无限期阻塞，并测试文件描述符变动 */
        result = select(FD_SETSIZE, &testfds, (fd_set *)0,(fd_set *)0, (struct timeval *) 0); //FD_SETSIZE：系统默认的最大文件描述符
        PJ_LOG(1, ("test", "select result:%d", result));
        if(result < 1)
        {
            perror("server5");
            exit(1);
        }

        /*找到相关文件描述符*/
        if (FD_ISSET(ss, &testfds)) {
            int nread;
            ioctl(ss, FIONREAD, &nread);//取得数据量交给nread
            char recvdata[512+4];
            pj_memset(recvdata, 0, sizeof(recvdata));
            pj_ssize_t received = sizeof(received);
            rc = pj_sock_recv(ss, recvdata, &received, 0);
            if (rc != PJ_SUCCESS) {
                app_perror("...recv error", rc);
                rc = -155;
            } else {
                PJ_LOG(1, ("", "ss received:%d data:%s  nread:%d", received, recvdata, nread));
            }
        }
        if (FD_ISSET(sss[0], &testfds)) {
            int nread;
            ioctl(sss[0], FIONREAD, &nread);//取得数据量交给nread
            char recvdata[512+4];
            pj_memset(recvdata, 0, sizeof(recvdata));
            pj_ssize_t received = sizeof(received);
            rc = pj_sock_recv(sss[0], recvdata, &received, 0);
            if (rc != PJ_SUCCESS) {
                app_perror("...recv error", rc);
                rc = -155;
            } else {
                PJ_LOG(1, ("", "received:%d data:%s  nread:%d", received, recvdata, nread));
            }
        }
        if (FD_ISSET(sss[1], &testfds)) {
            char recvdata[512+4];
            pj_memset(recvdata, 0, sizeof(recvdata));
            pj_ssize_t received = sizeof(received);
            rc = pj_sock_recv(sss[1], recvdata, &received, 0);
            if (rc != PJ_SUCCESS) {
                app_perror("...recv error", rc);
                rc = -155;
            } else {
                PJ_LOG(1, ("", "1received:%d data:%s", received, recvdata));
            }
        }
        if (FD_ISSET(sss[2], &testfds)) {
            char recvdata[512+4];
            pj_memset(recvdata, 0, sizeof(recvdata));
            pj_ssize_t received = sizeof(received);
            rc = pj_sock_recv(sss[2], recvdata, &received, 0);
            if (rc != PJ_SUCCESS) {
                app_perror("...recv error", rc);
                rc = -155;
            } else {
                PJ_LOG(1, ("", "2received:%d data:%s", received, recvdata));
            }
        }
    }

    return rc;
}

int pj_trans::do_iobuffer() {
    pj_status_t rc;

    pj_sockaddr_in addr;
    int addrlen;
    pj_ssize_t bytes;

    pj_bzero(&addr, sizeof(addr));
    addrlen = sizeof(addr);
    bytes = bufsize;
    rc = pj_ioqueue_recvfrom(skey, &read_op, recv_buf, &bytes, 0,
                             &addr, &addrlen);
    if (rc != PJ_SUCCESS && rc != PJ_EPENDING) {
        app_perror("...error: pj_ioqueue_recvfrom", rc);
    } else if (rc == PJ_EPENDING) {
        PJ_LOG(3, (THIS_FILE,
                "......ok: recvfrom returned pending"));
    } else {
        PJ_LOG(3, (THIS_FILE,
                "......error: recvfrom returned immediate ok!"));
    }

    return rc;
}