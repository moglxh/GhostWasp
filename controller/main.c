#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../include/gw_protocol.h"
#include "../include/gw_net.h"

int main(void)
{
	int server = socket(AF_INET, SOCK_STREAM, 0);
	if (server < 0) { perror("socket"); return 1; }

	struct sockaddr_in  address = {0};

	address.sin_family = AF_INET;
	address.sin_port = htons(4444);
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if (bind(server, (struct sockaddr *)&address, sizeof address) < 0) { perror("bind"); close(server); return 1; }
	if (listen(server, 1) < 0) { perror ("listen"); close(server); return 1; }

	printf("Ghostwasp controller listening on 127.0.0.1:4444\n");

	int agent = accept(server, NULL, NULL);

	if (agent < 0) { perror("accept"); close(server); return 1; }

	printf("Agent connected!\n");

	uint8_t header[3];

	ssize_t n = gw_recv_all(agent, header, sizeof header);
	if (n < 0) { perror("recv"); close(agent); close(server); return 1; }

	uint16_t payload_length;
	memcpy(&payload_length, header + 1, sizeof payload_length);


	uint8_t payload[GW_MAX_PAYLOAD];
	ssize_t payload_recieved = gw_recv_all(agent, payload, payload_length);


	struct gw_message message = {0};

	message.type = header[0];
	message.length = payload_length;

	memcpy(message.payload, payload, payload_length);

	if (message.type == GW_MSG_HELLO) {
    printf("Recieved HELLO from agent\n");
}

	close(agent);
	close(server);

	return 0;

}
