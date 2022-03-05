#include <string.h>
#include <stdio.h>
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
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "my.sock");

    socklen_t len = sizeof(addr);
    int ret = connect(fd, (struct sockaddr*)&addr, len);
    if(ret < 0)
    {
        perror("connect");
        return 0;
    }

    while(1)
    {
        char msg[] = "hello world!";
        send(fd, msg, sizeof(msg), 0);

        sleep(1);
    }

    close(fd);

    return 0;    
}