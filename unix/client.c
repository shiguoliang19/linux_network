#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        printf("sock_client [path] [msg] [size]\n");
        return 0;
    }
    char* path = argv[1];
    char* msg = argv[2];
    int size = atoi(argv[3]);

    int ret = 0;

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd < 0)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, path);

    socklen_t len = sizeof(addr);
    ret = connect(fd, (struct sockaddr*)&addr, len);
    if(ret < 0)
    {
        perror("connect");
        return 0;
    }

    for(int i = 0; i < size; ++i)
    {
        ret = write(fd, msg, sizeof(msg));
        if(ret < 0)
        {
            perror("write");
        }

        printf("send: %s\n", msg);
        sleep(1);
    }

    close(fd);

    return 0;    
}