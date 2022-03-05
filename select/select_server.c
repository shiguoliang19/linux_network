#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int ret = 0;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8080);
    ret = bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        perror("bind");
        return 0;
    }

    ret = listen(server_fd, 5);
    if(ret < 0)
    {
        perror("listen");
        return 0;
    }

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    uint64_t number = 0;
    while (1)
    {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        FD_SET(server_fd, &readfds);

        ret = select(FD_SETSIZE, &readfds, &writefds, &exceptfds, NULL);
        if (ret < 0)
        {
            perror("select");
            exit(1);
        }

        for (int fd = 0; fd < FD_SETSIZE; ++fd)
        {
            if (FD_ISSET(fd, &readfds))
            {
                // 新的连接
                if (fd == server_fd)
                {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                    if (client_fd < 0)
                    {
                        perror("accept");
                        break;
                    }

                    FD_SET(client_fd, &readfds);
                    printf("fd set: %d\n", client_fd);
                }
                // 读取数据
                else
                {
                    int nread;
                    ioctl(fd, FIONREAD, &nread);
                    if (nread == 0)
                    {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("fd clr: %d\n", fd);
                    }
                    else
                    {
                        char buffer[1024];
                        read(fd, &buffer, nread);
                        printf("number: %d, fd: %d, nread: %d, recv: %s\n", ++number, fd, nread, buffer);
                    }
                }
            }
        }

        sleep(1);
    }
    return 0;
}
