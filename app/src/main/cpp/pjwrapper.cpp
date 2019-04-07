//
// Created by Jl C on 2019/4/6.
//

#include "pjwrapper.h"
#include "jni_log.h"
#include "pj_utils.h"
#include "pjcommon/pj_trans.h"
#include <pjlib.h>

pj_caching_pool caching_pool;
pj_pool_factory *mem;
const char *filename;
int line;

int param_echo_sock_type;
//const char *param_echo_server = ECHO_SERVER_ADDRESS;
//int param_echo_port = ECHO_SERVER_START_PORT;
int param_log_decor = PJ_LOG_HAS_NEWLINE | PJ_LOG_HAS_TIME |
                      PJ_LOG_HAS_MICRO_SEC;


int pjwrapper_init() {
    pj_status_t rc;

    const char *filename;
    int line;

    mem = &caching_pool.factory;

#ifdef DEBUG
    pj_log_set_level(PJ_LOG_MAX_LEVEL);
#else
    pj_log_set_level(3);
#endif
    pj_log_set_decor(param_log_decor);
    pj_log_set_log_func(jni_log_func);

    rc = pj_init();
    if (rc != 0) {
        app_perror("pj_init() error!!", rc);
        return rc;
    }

    //pj_dump_config();
    pj_caching_pool_init( &caching_pool, NULL, 0 );

    PJ_LOG(3, ("test", "mem:%p", mem));


    return rc;
}

int pjwrapper_uninit() {
    pj_status_t rc;

    pj_caching_pool_destroy( &caching_pool );

    PJ_LOG(3,("test", ""));

    pj_thread_get_stack_info(pj_thread_this(), &filename, &line);
    PJ_LOG(3,("test", "Stack max usage: %u, deepest: %s:%u",
            pj_thread_get_stack_max_usage(pj_thread_this()),
            filename, line));
    if (rc == 0)
        PJ_LOG(3,("test", "Looks like everything is okay!.."));
    else
        PJ_LOG(3,("test", "Test completed with error(s)"));

    pj_shutdown();

    return rc;
}

pj_trans *trans;

int pjwrapper_start() {
    pj_status_t rc = PJ_SUCCESS;
    FUNC_ENTER

    trans = new pj_trans(mem);

    FUNC_EXIT
    return 0;
}

int pjwrapper_stop() {
    pj_status_t rc = PJ_SUCCESS;
    FUNC_ENTER

    delete trans;
    trans = NULL;

    FUNC_EXIT
    return 0;
}

int pjwrapper_action() {
    FUNC_ENTER
    pj_status_t rc;
    pj_sockaddr_in dstaddr;
    pj_str_t s;
    pj_bzero(&dstaddr, sizeof(dstaddr));
    dstaddr.sin_family = pj_AF_INET();
    dstaddr.sin_port = pj_htons(trans->port);
    dstaddr.sin_addr = pj_inet_addr(pj_cstr(&s, trans->ipaddr.c_str()));

    std::string test = "test";
    pj_ssize_t len = test.size();
    if(trans) {
        rc = trans->sendto(test.c_str(), &len, &dstaddr, sizeof(dstaddr));
    }

    FUNC_EXIT
    return rc;
}