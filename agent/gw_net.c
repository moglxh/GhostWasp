#include "../include/gw_net.h"

#include <errno.h>
#include <sys/socket.h>

ssize_t gw_send_all(int fd, const void *buffer, size_t length)
{
    const char *data = buffer;
    size_t total = 0;

    while (total < length) {
        ssize_t n = send(fd, data + total, length - total, 0);

        if (n < 0) {
            if (errno == EINTR)
                continue;

            return -1;
        }

        if (n == 0)
            break;

        total += n;
    }

    return (ssize_t)total;
}

ssize_t gw_recv_all(int fd, void *buffer, size_t length)
{
    char *data = buffer;
    size_t total = 0;

    while (total < length) {
        ssize_t n = recv(fd, data + total, length - total, 0);

        if (n < 0) {
            if (errno == EINTR)
                continue;

            return -1;
        }

        if (n == 0)
            break;

        total += n;
    }

    return (ssize_t)total;
}
