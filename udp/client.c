#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	int sockfd = -1;
	char* msg = "ping";
	int retval = -1;
	char buf[BUFSIZ] = {};
	int len = -1;
	struct sockaddr_in cli_addr = {};
	socklen_t cli_addr_len = sizeof(cli_addr);
	struct sockaddr_in srv_addr = {};
	socklen_t srv_addr_len = sizeof(srv_addr);

	// socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	// cli addr
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	cli_addr.sin_port = htons(8080);

	// send
	len = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&cli_addr, cli_addr_len);
	if(len < 0)
	{
		perror("sendto");
		return -1;
	}
	printf("sendto: [%s] len: %d\n", msg, len);

	// recv
	len = recvfrom(sockfd, buf, BUFSIZ, 0, (struct sockaddr*)&srv_addr, &srv_addr_len);
	if(len < 0)
	{
		perror("recvfrom");
		return -1;
	}
	printf("recvfrom: [%s] len: %d\n", buf, len);

	retval = close(sockfd);
	if(retval < 0)
	{
		perror("close");
		return -1;
	}

	return 0;
}
