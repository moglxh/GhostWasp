#ifndef GW_PROTOCOL_H
#define GW_PROTOCOL_H

#include <stdint.h>

#define GW_MSG_HELLO 0x01

struct gw_message {
    uint8_t type;
    uint16_t length;
    char payload[64];
};

#endif
