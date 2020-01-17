#include "../include/tcp.h"

int cun_connection_init(struct cun_connection *connection)
{
    memset(connection, 0, sizeof(*connection));

    return 0;
}

int cun_connection_accept(struct cun_server *server, struct cun_connection *connection)
{
    cun_connection_init(connection);

    socklen_t length = sizeof(connection->address);
    do
    {
        connection->file = accept4(server->connection.file, 
                                   &(connection->address.common_address), &length, SOCK_NONBLOCK);
        if(connection->file >= 0)
        {
            int result = epoll_ctl(server->event_base, EPOLL_CTL_ADD, connection->file, &connection->event);
            if(result < 0)
            {
                cun_connection_shutdown(connection);
                return -1;
            }
            return 0;
        }
    } while(CUN_ACCEPT_AGAIN);

    return -1;
}