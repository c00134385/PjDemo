//
// Created by Jl C on 2019/4/6.
//

#include "pjwrapper.h"
#include "pjutils.h"
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

    pj_log_set_level(3);
    pj_log_set_decor(param_log_decor);
    pj_log_set_log_func(app_log_func);

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

int pjwrapper_start() {
    FUNC_ENTER

    FUNC_EXIT
    return 0;
}

int pjwrapper_stop() {
    FUNC_ENTER

    FUNC_EXIT
    return 0;
}

int pjwrapper_action() {
    FUNC_ENTER

    FUNC_EXIT
    return 0;
}