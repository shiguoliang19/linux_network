#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

int main()
{
    struct pollfd fds[1024];

    int listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        printf("socket exit code: %d\n", listen_fd);
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(8080);
    inet_pton(PF_INET, "0.0.0.0", &addr.sin_addr);
    int ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        printf("bind exit code: %d\n", listen_fd);
        exit(1);
    }
    int ret2 = listen(listen_fd, 5);
    if (ret2 < 0)
    {
        printf("listen exit code: %d\n", ret2);
        exit(1);
    }

    fds[0].fd = listen_fd;
    fds[0].events = POLLIN | POLLOUT | POLLERR;
    fds[0].revents = 0;

    int count = 0;
    while (1)
    {
        int ret = poll(fds, count + 1, -1);
        if (ret < 0)
        {
            printf("poll exit code: %d\n", ret);
            continue;
        }

        for (int i = 0; i < count + 1; ++i)
        {
            // 客户端关闭
            if (fds[i].revents & POLLHUP || fds[i].revents & POLLERR)
            {
                int fd = fds[i].fd;
                close(fd);

                bool movable = 0;
                for (int j = 0; j < count + 1; ++j)
                {
                    if (fd == fds[j].fd)
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

                printf("socket close\n");
            }
            // 新的连接
            else if (fds[i].fd == listen_fd && fds[i].revents & POLLIN)
            {
                struct sockaddr_in addr;
                int len = sizeof(addr);
                int client_fd = accept(listen_fd, (struct sockaddr *)&addr, &len);
                if (client_fd < 0)
                {
                    printf("accept exit code: %d\n", client_fd);
                    continue;
                };

                ++count;

                fds[count].fd = client_fd;
                fds[count].events = POLLIN | POLLOUT | POLLERR;
                fds[count].revents = 0;

                printf("new connect\n");
            }
            // 有可读数据
            else if (fds[i].revents & POLLIN)
            {
                char buffer[1024];
                int ret = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (ret < 0)
                {
                    printf("recv exit code: %d\n", ret);
                    continue;
                }
                printf("data: %s\n", buffer);
            }
            // 有可写数据
            else if (fds[i].revents & POLLOUT)
            {
                char msg[] = "hello world\n";
                int ret = send(fds[i].fd, msg, sizeof(msg), 0);
                if (ret < 0)
                {
                    printf("send exit code: %d\n", ret);
                    continue;
                }
            }
        }
    }

    return 0;
}
