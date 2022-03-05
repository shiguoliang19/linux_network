#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/ioctl.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("42.192.157.201");
    addr.sin_port = htons(8082);
    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        perror("connect");
        return 0;
    }

    char msg[] = "I am is client.";
    uint16_t len = sizeof(msg);
    while (1)
    {
        int nread = 0;
        ioctl(fd, FIONREAD, &nread);
        if (nread > 0)
        {
            char *buffer = malloc(nread);
            read(fd, buffer, nread);
            printf("recv: %s\n", buffer);
            free(buffer);
        }

        write(fd, &msg, len);
        printf("send: %s\n", msg);

        sleep(1);
    }

    close(fd);

    return 0;
}
