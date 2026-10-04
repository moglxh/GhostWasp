#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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

	const char *message = "HELLO";

	if (send(fd, message, 5, 0) < 0) { perror("send"); close(fd); return 1; }

	printf("sent: %s\n", message);

	close(fd);

	return 0;

}
