#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <string.h>

int main()
{
    int sockfd;
    unsigned short port;
    in_addr_t addr_in;
    struct sockaddr_in ser_addr;
    int len = 0;
    char send_buf[1024];
    char recv_buf[1024];
    int ret = -1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ser_addr.sin_port = htons(8000);
    ret = connect(sockfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    if (ret == -1)
    {
        perror("connect");
        return -1;
    }

    memset(&send_buf, 0, sizeof(send_buf));
    sprintf(send_buf, "PING!");
    len = send(sockfd, &send_buf, strlen(send_buf), 0);
    if(len == -1)
    {
        perror("send");
        return -1;
    }

    memset(&recv_buf, 0, sizeof(recv_buf));
    printf("Now recv...\n");
    len = recv(sockfd, recv_buf, sizeof(recv_buf), 0);
    if(len == -1)
    {
        perror("recv");
        return -1;
    }

    printf("Receive message: %s\n", recv_buf);
    close(sockfd);

    return 0;
}
