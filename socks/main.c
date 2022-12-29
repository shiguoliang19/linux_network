#include <linux/if_tun.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "socks.h"

#define MAX_EVENTS 32

int epfd = 0;

void handle_auth_fn(sock5_proxy_t *sock5_proxy)
{
    auth_request_t auth_request;
    auth_reply_t auth_reply;
    ssize_t nbytes = 0;
    ssize_t nread = 0;
    ssize_t nwrite = 0;
    int fd = sock5_proxy->socket_pair.fd;

    nbytes = sizeof(auth_request.ver) + sizeof(auth_request.n_methods);
    nread = read(fd, (char *)&auth_request, nbytes);
    if (nread == -1)
    {
        perror("read");
        return;
    }
    else if (nread == 0)
    {
        return;
    }

    nread = read(fd, (char *)&auth_request + nbytes, auth_request.n_methods);
    if (nread == -1)
    {
        perror("read");
        return;
    }
    else if (nread == 0)
    {
        return;
    }

    printf("ver: %02x, ", auth_request.ver);
    printf("n_methods: %d, ", auth_request.n_methods);
    printf("methods:");
    for (int i = 0; i < auth_request.n_methods; ++i)
    {
        printf(" %02x", auth_request.methods[i]);
    }
    printf("\n");

    auth_handle_fn(&auth_request, &auth_reply);

    // auth reply
    sock5_proxy->wbuffer = malloc(sizeof(auth_reply_t));
    memcpy(sock5_proxy, &auth_reply, sizeof(auth_reply_t));

    sock5_proxy->sock5_process = SOCK5_PROXY;
}

void handle_proxy_fn(sock5_proxy_t *sock5_proxy)
{
    proxy_request_t proxy_request;
    proxy_reply_t proxy_reply;
    ssize_t nread = 0;
    ssize_t nwrite = 0;
    int fd = sock5_proxy->socket_pair.fd;

    nread = read(fd, (char *)&proxy_request, sizeof(proxy_request));
    if (nread == -1)
    {
        perror("read");
        return;
    }
    else if (nread == 0)
    {
        return;
    }

    printf("ver: %02x, ", proxy_request.ver);
    printf("cmd: %02x, ", proxy_request.cmd);
    printf("rsv: %02x, ", proxy_request.rsv);
    printf("atyp: %02x, ", proxy_request.atyp);
    printf("dst.addr: %s, ", inet_ntoa(proxy_request.dst_addr));
    printf("dst.port: %d\n", ntohs(proxy_request.dst_port));

    proxy_handle_fn(&proxy_request, &proxy_reply, epfd, sock5_proxy);

    // proxy reply
    sock5_proxy->wbuffer = malloc(sizeof(proxy_reply_t));
    memcpy(sock5_proxy, &proxy_reply, sizeof(proxy_reply_t));

    sock5_proxy->sock5_process = SOCK5_DATA;
}

void handle_data_fn(sock5_proxy_t *sock5_proxy)
{
    char buffer[BUFFER_SIZE];
    ssize_t nread;

    if (sock5_proxy->socket_type == SOCK5_PROXY)
    {
        nread = read(sock5_proxy->socket_pair.fd, buffer, BUFFER_SIZE);
        if (nread == -1)
        {
            perror("read");
            return;
        }
        else if (nread == 0)
        {
            return;
        }
    }
    else if (sock5_proxy->socket_type == SOCKET_SERVER)
    {
        nread = read(sock5_proxy->socket_pair.fd, buffer, BUFFER_SIZE);
        if (nread == -1)
        {
            perror("read");
            return;
        }
        else if (nread == 0)
        {
            return;
        }
    }

    printf("read server data. nread = %ld\n", nread);
    printf("data: ");
    for(int i = 0; i < nread; ++i)
    {
        printf(" %02x", (unsigned char)buffer[i]);
    }
    printf("\n");

    // forward
    sock5_proxy->wbuffer = malloc(nread);
    memcpy(sock5_proxy->wbuffer, buffer, nread);
}

void read_callback_fn(sock5_proxy_t *sock5_proxy)
{
    switch (sock5_proxy->sock5_process)
    {
    case SOCK5_AUTH:
        handle_auth_fn(sock5_proxy);
        break;
    case SOCK5_PROXY:
        handle_proxy_fn(sock5_proxy);
        break;
    case SOCK5_DATA:
        handle_data_fn(sock5_proxy);
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    int ret = ~0;

    // 创建监听socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror("socket");
        exit(0);
    }

    // 设置端口复用
    int optval = 1;
    ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
    if (ret < 0)
    {
        perror("setsockopt");
        exit(0);
    }

    // 绑定地址和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(1080);
    ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        perror("bind");
        exit(0);
    }

    // 监听，最多接收32个连接
    ret = listen(listen_fd, 32);
    if (ret < 0)
    {
        perror("listen");
        exit(0);
    }

    printf("listen_fd: %d\n", listen_fd);

    // 创建epoll
    epfd = epoll_create1(EPOLL_CLOEXEC);

    // 添加epoll事件
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT;
    sock5_proxy_t *sock5_proxy = malloc(sizeof(sock5_proxy_t));
    sock5_proxy->socket_type = SOCKET_LISTEN;
    sock5_proxy->sock5_process = SOCK5_LISTEN;
    sock5_proxy->socket_pair.fd = listen_fd;
    sock5_proxy->socket_pair.peer_fd = 0;
    sock5_proxy->wbuffer = NULL;
    sock5_proxy->len = 0;
    ev.data.ptr = sock5_proxy;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

    struct epoll_event events[MAX_EVENTS];
    while (1)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds < 0)
        {
            perror("epoll_wait");
            continue;
        }

        for (int i = 0; i < nfds; ++i)
        {
            printf("i: %d events: %08x\n", i, events[i].events);

            sock5_proxy_t *sock5_proxy = events[i].data.ptr;
            int fd = sock5_proxy->socket_pair.fd;

            // 错误处理
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP))
            {
                epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                free(sock5_proxy);
                printf("close socket. fd = %d\n", fd);
            }
            // 新的连接
            else if (fd == listen_fd)
            {
                struct sockaddr_in addr;
                socklen_t len = sizeof(struct sockaddr_in);
                int client_fd = accept(fd, (struct sockaddr *)&addr, &len);
                if (client_fd < 0)
                {
                    perror("accept");
                    continue;
                }

                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLOUT;
                sock5_proxy_t *sp = malloc(sizeof(sock5_proxy_t));
                sp->socket_type = SOCKET_PROXY;
                sp->sock5_process = SOCK5_AUTH;
                sp->socket_pair.fd = client_fd;
                sp->socket_pair.peer_fd = 0;
                sock5_proxy->wbuffer = NULL;
                sock5_proxy->len = 0;
                ev.data.ptr = sp;
                epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

                printf("new connect. ip: %s, port: %d\n", inet_ntoa(addr.sin_addr), addr.sin_port);
            }
            // 读取数据
            else if(events[i].events & EPOLLIN)
            {
                read_callback_fn(sock5_proxy);
            }
            // 写入数据
            else if(events[i].events & EPOLLOUT)
            {
                if(sock5_proxy->wbuffer == NULL && sock5_proxy->len == 0)
                    continue;
                
                write(fd, sock5_proxy->wbuffer, sock5_proxy->len);
                free(sock5_proxy->wbuffer);
                sock5_proxy->wbuffer = NULL;
                sock5_proxy->len = 0;
            }
        }
    }

    return 0;
}
