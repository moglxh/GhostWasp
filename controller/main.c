#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../include/gw_protocol.h"
#include "../include/gw_net.h"
#include "../include/gw_session.h"

int main(void)
{
    // Initialize session manager.
    struct gw_session_manager manager;

    if (gw_session_manager_init(&manager) < 0) {
        return 1;
    }

    // Create TCP listening socket.
    int server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0) {
        perror("socket");
        return 1;
    }

    // Configure localhost:4444.
    struct sockaddr_in address = {0};

    address.sin_family = AF_INET;
    address.sin_port = htons(4444);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // Bind the socket.
    if (bind(server,
             (struct sockaddr *)&address,
             sizeof address) < 0) {
        perror("bind");
        close(server);
        return 1;
    }

    // Start listening.
    if (listen(server, 1) < 0) {
        perror("listen");
        close(server);
        return 1;
    }

    printf("GhostWasp Controller listening on 127.0.0.1:4444\n");

    // Accept one Agent connection.
    int agent = accept(server, NULL, NULL);

    if (agent < 0) {
        perror("accept");
        close(server);
        return 1;
    }

    printf("Agent connected!\n");

    // Create a session for the Agent.
    struct gw_session *session =
        gw_session_create(agent, 1001);

    if (session == NULL) {
        perror("gw_session_create");
        close(agent);
        close(server);
        return 1;
    }

    // Register the session with the manager.
    if (gw_session_manager_add(&manager, session) < 0) {
        gw_session_destroy(session);
        close(server);
        return 1;
    }

    printf("Session created %u\n", session->id);

    // Receive the message header.
    uint8_t header[3];

    ssize_t n =
        gw_recv_all(session->fd, header, sizeof header);

    if (n < 0) {
        perror("recv");
        gw_session_manager_destroy(&manager);
        close(server);
        return 1;
    }

    // Extract payload length.
    uint16_t payload_length;

    memcpy(&payload_length,
           header + 1,
           sizeof payload_length);

    // Receive the payload.
    uint8_t payload[GW_MAX_PAYLOAD];

    ssize_t payload_received =
        gw_recv_all(session->fd,
                    payload,
                    payload_length);

    if (payload_received < 0) {
        perror("recv");
        gw_session_manager_destroy(&manager);
        close(server);
        return 1;
    }

    // Rebuild the message.
    struct gw_message message = {0};

    message.type = header[0];
    message.length = payload_length;

    memcpy(message.payload,
           payload,
           payload_length);

    // Handle the message.
    if (message.type == GW_MSG_HELLO) {
        printf("Session %u sent HELLO\n",
               session->id);
    }

// Save ID before destroying the session.
uint32_t session_id = session->id;

if (gw_session_manager_remove(&manager, session_id) < 0) {
    fprintf(stderr, "Failed to remove session %u\n",
            session_id);
    gw_session_manager_destroy(&manager);
    close(server);
    return 1;
}

printf("Session %u removed\n", session_id);

// Close listening socket.
close(server);

    return 0;
}
