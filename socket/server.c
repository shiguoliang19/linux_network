#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

int main()
{
    int listenfd, connfd;
    struct sockaddr_in srv_addr, cli_addr;
    socklen_t len = 0;
    char recv_buf[1024], send_buf[1024];
    int ret = -1;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
    {
        perror("socket");
        return -1;
    }

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(8000);
    ret = bind(listenfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (ret < 0)
    {
        perror("bind");
        return -1;
    }

    printf("Now listen...\n");
    ret = listen(listenfd, 5);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    while (1)
    {
        memset(&cli_addr, 0, sizeof(cli_addr));
        len = sizeof(cli_addr);
        printf("Now accept...\n");
        connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &len);
        if (connfd < 0)
        {
            perror("accept");
            break;
        }

        printf("Now recv...\n");
        memset(recv_buf, 0, sizeof(recv_buf));
        len = recv(connfd, recv_buf, sizeof(recv_buf), 0);
        if(len < 0)
        {
            perror("recv");
            return -1;
        }
        printf("Recv form client is: %s\n", recv_buf);

        printf("Now send...\n");
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "PONG!");
        len = send(connfd, send_buf, strlen(send_buf), 0);
        if(len < 0)
        {
            perror("send");
            return -1;
        }

        close(connfd);
    }
    close(listenfd);
    return 0;
}
