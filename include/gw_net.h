#ifndef GW_NET_H
#define GW_NET_H

#include <stddef.h>
#include <sys/types.h>

ssize_t gw_send_all(int fd, const void *buffer, size_t length);
ssize_t gw_recv_all(int fd, void *buffer, size_t length);

#endif
