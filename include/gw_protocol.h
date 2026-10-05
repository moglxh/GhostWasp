#ifndef GW_PROTOCOL_H
#define GW_PROTOCOL_H

#include <stdint.h>

#define GW_MSG_HELLO 0x01
#define GW_MAX_PAYLOAD 64

struct gw_message {
    uint8_t type;
    uint16_t length;
    char payload[GW_MAX_PAYLOAD];
};

int gw_message_encode(const struct gw_message *message,
                      uint8_t *buffer);

int gw_message_decode(const uint8_t *buffer, uint16_t buffer_size, struct gw_message *message);

#endif
