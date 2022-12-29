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
    struct sockaddr_in cli_addr, srv_addr;
    socklen_t len = 0;
    char send_buf[1024];
    char recv_buf[1024];
    int ret;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
    {
        perror("socket");
        return -1;
    }

    int opt =1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

    ret = listen(listenfd, 5);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    while (1)
    {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        FD_SET(listenfd, &readfds);

        ret = select(FD_SETSIZE, &readfds, &writefds, &exceptfds, NULL);
        if (ret < 0)
        {
            perror("select");
            continue;
        }

        for (int fd = 0; fd < FD_SETSIZE; ++fd)
        {
            if (FD_ISSET(fd, &readfds) && !FD_ISSET(fd, &writefds) && !FD_ISSET(fd, &exceptfds))
            {
                if (fd == listenfd)
                {
                    memset(&cli_addr, 0, sizeof(cli_addr));
                    len = sizeof(cli_addr);
                    connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &len);
                    if (connfd < 0)
                    {
                        perror("accept");
                        break;
                    }
                    FD_SET(connfd, &readfds);
                    printf("FD_SET: %d\n", connfd);
                }
                else
                {
                    printf("Now recv...\n");
                    memset(recv_buf, 0, sizeof(recv_buf));
                    len = recv(fd, recv_buf, sizeof(recv_buf), 0);
                    if(len < 0)
                    {
                        perror("recv");
                        return -1;
                    }
                    printf("Recv form client is: %s\n", recv_buf);

                    printf("Now send...\n");
                    memset(send_buf, 0, sizeof(send_buf));
                    sprintf(send_buf, "PONG!");
                    len = send(fd, send_buf, strlen(send_buf), 0);
                    if(len < 0)
                    {
                        perror("send");
                        return -1;
                    }

                    close(fd);
                    FD_CLR(fd, &readfds);
                    printf("FD_CLR: %d\n", fd);       
                }
            }
        }

        sleep(1);
    }
    return 0;
}
