#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#define CUN_CONNECTION_PREALLOCATE 8192
#define CUN_SERVER_CONNECTION_PULL_SIZE 1000000

#define CUN_TCP_ACCEPT_AGAIN    \
    ((EAGAIN       == errno) || \
     (EINTR        == errno) || \
     (ENETDOWN     == errno) || \
     (EPROTO       == errno) || \
     (ENOPROTOOPT  == errno) || \
     (EHOSTDOWN    == errno) || \
     (ENONET       == errno) || \
     (EHOSTUNREACH == errno) || \
     (EOPNOTSUPP   == errno) || \
     (ENETUNREACH  == errno))

#define CUN_TCP_READ_AGAIN      \
    ((EAGAIN       == errno) || \
     (EINTR        == errno))

struct cun_connection
{
    int file;
    struct epoll_event event;
    
    union {
        struct sockaddr common_address;
        struct sockaddr_in ipv4_address;
        struct sockaddr_in6 ipv6_address;
    } address;

    // use as "small buffer" or high level protocol header
    char small[CUN_CONNECTION_PREALLOCATE];
    char small_size; 
};

struct cun_server
{
    struct cun_connection connection;
    int event_base;
    struct cun_connection connection_pool[CUN_SERVER_CONNECTION_PULL_SIZE];
};

int cun_connection_accept(struct cun_server *server, struct cun_connection *connection);
int cun_connection_shutdown(struct cun_server *server, struct cun_connection *connection);
