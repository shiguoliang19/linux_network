#include "socks.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void auth_handle_fn(const auth_request_t *request, auth_reply_t *reply)
{
    reply->ver = 0x05;
    reply->method = 0x00;
}

void proxy_handle_fn(const proxy_request_t *request, proxy_reply_t *reply, int epfd, sock5_proxy_t* sock5_proxy)
{
    int fd = ~0;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(0);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr = request->dst_addr;
    addr.sin_port = request->dst_port;
    socklen_t len = sizeof(addr);
    int err = connect(fd, (struct sockaddr *)&addr, len);
    if (err < 0)
    {
        perror("connect");
        exit(0);
    }

    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    getsockname(fd, (struct sockaddr *)&client_addr, &client_len);

    reply->ver = 0x05;
    reply->rep = 0x00;
    reply->atyp = 0x01;
    
    reply->bnd_addr.s_addr = INADDR_ANY;
    reply->bnd_port = 0;

    // proxy
    sock5_proxy->socket_pair.peer_fd = fd;

    // server
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT;
    sock5_proxy_t* sp = malloc(sizeof(sock5_proxy_t));
    sp->socket_type = SOCKET_SERVER;
    sp->sock5_process = SOCK5_DATA;
    sp->socket_pair.fd = fd;
    sp->socket_pair.peer_fd = sock5_proxy->socket_pair.fd;
    sock5_proxy->wbuffer = NULL;
    sock5_proxy->len = 0;
    ev.data.ptr = sp;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}