#include "../include/gw_protocol.h"

#include <string.h>

int gw_message_encode(const struct gw_message *message,
                      uint8_t *buffer)
{
    buffer[0] = message->type;

    uint16_t length = message->length;

    memcpy(buffer + 1, &length, sizeof length);

    memcpy(buffer + 3,
           message->payload,
           message->length);

    return 3 + message->length;
}

int gw_message_decode(const uint8_t *buffer,
                      uint16_t buffer_size,
                      struct gw_message *message)
{
    if (buffer_size < 3)
        return -1;

    message->type = buffer[0];

    uint16_t length;

    memcpy(&length, buffer + 1, sizeof length);

    message->length = length;

    if (message->length > GW_MAX_PAYLOAD)
        return -1;

    if (buffer_size < 3 + message->length)
        return -1;

    memcpy(message->payload,
           buffer + 3,
           message->length);

    return 0;
}
