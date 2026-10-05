#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../include/gw_protocol.h"
#include "../include/gw_net.h"

int main(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) { perror("socket"); return 1; }

	struct sockaddr_in srv = {0};
	srv.sin_family = AF_INET;
	srv.sin_port = htons(4444);
	inet_pton(AF_INET, "127.0.0.1", &srv.sin_addr);

	if (connect(fd, (struct sockaddr *)&srv, sizeof srv) < 0) { perror("connect"); close(fd); return 1;
 }
	printf("Connected to Ghostwasp controller!\n");
	struct gw_message message = { .type = GW_MSG_HELLO, .length = 5, .payload = "HELLO" };


	uint8_t buffer[67];

	int length = gw_message_encode(&message, buffer); 

	if (gw_send_all(fd, buffer, length) != length) { perror("send"); close(fd); return 1; }

	printf("Sent Hello\n");

	close(fd);

	return 0;

}
