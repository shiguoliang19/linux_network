#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd < 0)
    {
        perror("socket");
        return 0;
    }
    
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "my.sock");

    socklen_t len = sizeof(addr);
    int ret = bind(fd, (struct sockaddr*)&addr, len);
    if(ret < 0)
    {
        perror("bind");
        return 0;
    }

    listen(fd, 5);

    struct sockaddr_un client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
    if(client_fd < 0)
    {
        perror("accept");
        return 0;
    }

    while(1)
    {
        char buffer[1024];
        int len = recv(client_fd, buffer, sizeof(buffer), 0);
        if(len < 0)
        {
            perror("recv error");
            exit(0);
        }
        else if(len == 0)
        {
            perror("recv close");
            break;
        }
        else if(len > 0)
        {
            printf("recv: %s\n", buffer);
        }
    }

    close(fd);
    close(client_fd);

    return 0;
}