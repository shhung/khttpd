#ifndef KHTTPD_HTTP_SERVER_H
#define KHTTPD_HTTP_SERVER_H

#include <linux/module.h>
#include <linux/workqueue.h>
#include <net/sock.h>

struct http_server_param {
    struct socket *listen_socket;
};

struct http_service {
    bool is_stopped;
    struct list_head worker;
};

struct khttpd {
    struct socket *sock;
    struct list_head list;
    struct work_struct khttpd_work;
};

extern int http_server_daemon(void *arg);

enum {
    TRACE_accept_err = 1,  // accept 失敗總數
    TRACE_cthread_err,     // create thread 失敗總數
    TRACE_kmalloc_err,     // kmalloc 失敗總數
    TRACE_recvmsg,         // recvmsg 總數
    TRACE_sendmsg,         // sendmsg 總數
    TRACE_send_err,        // send request 失敗總數
    TRACE_recv_err,        // recv request 失敗總數
};

struct runtime_state {
    atomic_t accept_err, cthread_err;
    atomic_t kmalloc_err, recvmsg;
    atomic_t sendmsg, send_err;
    atomic_t recv_err;
};
extern struct runtime_state states;

#define TRACE(ops)                      \
    do {                                \
        if (TRACE_##ops)                \
            atomic_add(1, &states.ops); \
    } while (0)

#endif
