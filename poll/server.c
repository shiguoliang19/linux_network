#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int sockfd;
    int cli_fd, srv_fd;
    struct pollfd fds[1024];
    struct sockaddr_in cli_addr, srv_addr;
    char send_buf[1024], recv_buf[1024];
    socklen_t len;
    int ret;
    int count = 0;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = PF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(8000);
    ret = bind(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (ret < 0)
    {
        perror("bind");
        return -1;
    }

    ret = listen(sockfd, 5);
    if (ret < 0)
    {
        perror("listen");
        return -1;
    }

    fds[0].fd = sockfd;
    fds[0].events = POLLIN | POLLOUT | POLLHUP | POLLERR;
    fds[0].revents = 0;
    while (1)
    {
        int ret = poll(fds, count + 1, -1);
        if (ret < 0)
        {
            perror("poll");
            continue;
        }

        for (int i = 0; i < count + 1; ++i)
        {
            if (fds[i].fd == sockfd && fds[i].revents & POLLIN)
            {
                len = sizeof(cli_addr);
                cli_fd = accept(sockfd, (struct sockaddr *)&cli_addr, &len);
                if (cli_fd < 0)
                {
                    perror("accept");
                    continue;
                };

                ++count;
                fds[count].fd = cli_fd;
                fds[count].events = POLLIN | POLLOUT | POLLHUP | POLLERR;
                fds[count].revents = 0;

                printf("New Connect : ip: %s, port: %d\n", inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);
            }
            if (fds[i].revents & POLLHUP || fds[i].revents & POLLERR)
            {
                cli_fd = fds[i].fd;
                len = sizeof(cli_addr);
                getpeername(cli_fd, (struct sockaddr *)&cli_addr, &len);
                shutdown(cli_fd, SHUT_RDWR);

                bool movable = 0;
                for (int j = 0; j < count + 1; ++j)
                {
                    if (cli_fd == fds[j].fd)
                    {
                        movable = true;
                        continue;
                    } 
                    if (movable)
                    {
                        fds[j - 1] = fds[j];
                    }
                }
                --count;

                printf("Disconnect from client: ip: %s, port: %d\n", inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);
            }
            else
            {
                if (fds[i].revents & POLLIN)
                {
                    printf("Now recv...\n");
                    memset(recv_buf, 0, sizeof(recv_buf));
                    len = recv(fds[i].fd, recv_buf, sizeof(recv_buf), 0);
                    if(len < 0)
                    {
                        perror("recv");
                        return -1;
                    }
                    printf("Recv form client is: %s\n", recv_buf);

                    printf("Now send...\n");
                    memset(send_buf, 0, sizeof(send_buf));
                    sprintf(send_buf, "PONG!");
                    len = send(fds[i].fd, send_buf, strlen(send_buf), 0);
                    if(len < 0)
                    {
                        perror("send");
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}
