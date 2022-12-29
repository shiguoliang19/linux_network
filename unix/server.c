#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/ioctl.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("sock_server [path]\n");
        return 0;
    }
    char* path = argv[1];

    int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        perror("socket");
        return 0;
    }
    
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);

    socklen_t len = sizeof(addr);
    int ret = bind(listen_fd, (struct sockaddr*)&addr, len);
    if(ret < 0)
    {
        perror("bind");
        return 0;
    }

    listen(listen_fd, 5);

    struct sockaddr_un client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if(client_fd < 0)
    {
        perror("accept");
        return 0;
    }

    while(1)
    {
        int nread = 0;
        if(ioctl(client_fd, FIONREAD, &nread) < 0)
            continue;

        char* msg = malloc(nread);
        int len = read(client_fd, msg, sizeof(msg));
        if(len < 0)
        {
            perror("recv error");
            free(msg);
            break;
        }
        else if(len == 0)
        {
            perror("recv close");
            free(msg);
            break;
        }
        else if(len > 0)
        {
            printf("recv: %s\n", msg);
        }

        free(msg);
    }

    close(client_fd);
    close(listen_fd);

    return 0;
}