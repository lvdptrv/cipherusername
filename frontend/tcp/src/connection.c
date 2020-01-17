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
        if (connection->file >= 0)
        {
            int result = epoll_ctl(server->event_base, EPOLL_CTL_ADD, connection->file, &connection->event);
            if (result < 0)
            {
                cun_connection_shutdown(server, connection);
                return -1;
            }
            return 0;
        }
    } while (CUN_TCP_ACCEPT_AGAIN);

    return -1;
}

int cun_connection_shutdown(struct cun_server *server,
                            struct cun_connection *connection)
{
    epoll_ctl(server->event_base, EPOLL_CTL_DEL, connection->file,
              &connection->event);

    if (NULL != connection->event.data.ptr)
    {
        free(connection->event.data.ptr);
        connection->event.data.ptr = NULL;
    }

    // TODO: I don't known how handle close error :(
    close(connection->file);
    shutdown(connection->file, SHUT_RDWR);
}

ssize_t cun_connection_read(struct cun_connection *connection, char *buffer_if_outside, ssize_t outside_buffer_size)
{
    socklen_t length = sizeof(connection->address);
    char *destination = buffer_if_outside == NULL ? connection->small : buffer_if_outside;
    size_t buffer_size = buffer_size < 0 ? sizeof(connection->small) : outside_buffer_size;

    ssize_t read_size = 0;
    size_t read_total = 0;
    do
    {
        read_size = recvfrom(connection->file, destination + read_total,
                             buffer_size - read_total, MSG_NOSIGNAL,
                             &connection->address, &length);
        if (read_size > 0)
        {
            read_total += read_size;
            if(NULL != buffer_if_outside) {
                connection->small_size += read_size;
            }
        } else if((read_size == 0) || !CUN_TCP_READ_AGAIN) {
            break;   
        }
    } while (read_size > 0);

    return read_total;
}