#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <s

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_EVENTS 32
#define TIMEOUT 1000

int main()
{
    // 创建socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 绑定
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);
    bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 监听
    listen(listen_fd, 64);

    // 创建epoll
    int epfd = epoll_create1(EPOLL_CLOEXEC);

    // 添加epoll事件
    struct epoll_event ev;
    ev.data.fd = listen_fd;
    ev.events = EPOLLIN | EPOLLOUT;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

    struct epoll_event events[MAX_EVENTS];
    while (1)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, TIMEOUT);
        if (nfds < 0)
        {
            perror("epoll_wait");
            continue;
        }

        for (int i = 0; i < nfds; ++i)
        {
            // 新的连接
            if (events[i].data.fd == listen_fd)
            {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);

                ev.data.fd = fd;
                ev.events = EPOLLIN | EPOLLOUT;
                epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

                printf("new connect: ip: %s, port: %d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
            }
            // 接收数据
            else if (events[i].events & EPOLLIN)
            {
                int fd = events[i].data.fd;

                int nread = 0;
                ioctl(fd, FIONREAD, &nread);
                if (nread > 0)
                {
                    char *buffer = malloc(nread);
                    read(fd, buffer, nread);
                    printf("recv data: %s\n", buffer);
                    free(buffer);
                }
            }
            // 发送数据
            else if (events[i].events & EPOLLOUT)
            {
                int fd = events[i].data.fd;

                char msg[] = "I am is server.";

                write(fd, msg, sizeof(msg));
                printf("send data: %s\n", msg);

                sleep(1);
            }
            // 错误处理
            else if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP)
            {
                int fd = events[i].data.fd;
                close(fd);

                ev.data.fd = fd;
                epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);

                printf("close socket. fd = %d\n", fd);
            }
        }
    }

    return 0;
}
