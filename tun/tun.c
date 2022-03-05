#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

int tun_alloc(char *dev, int flags)
{
    struct ifreq ifr;
    int fd;
    int err;
    char *clonedev = "/dev/net/tun";

    if ((fd = open(clonedev, O_RDWR)) < 0)
    {
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = flags;

    if (*dev)
    {
        strncpy(ifr.ifr_name, dev, 16);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0)
    {
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);

    return fd;
}

int createClientSocket(char *sin_addr, uint16_t sin_port)
{
    int ret;

    int net_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (net_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(sin_addr);
    addr.sin_port = htons(sin_port);
    int err = connect(net_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0)
    {
        perror("connect");
        exit(1);
    }

    return net_fd;
}

int createServerSocket(char *sin_addr, uint16_t sin_port)
{
    int ret = 0;

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(sin_addr);
    addr.sin_port = htons(sin_port);
    ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        perror("bind");
        exit(1);
    }

    ret = listen(listen_fd, 5);
    if (ret < 0)
    {
        perror("listen");
        exit(1);
    }

    // wait a new connect
    int net_fd = -1;
    while (1)
    {
        fd_set rd_set;

        FD_ZERO(&rd_set);
        FD_SET(listen_fd, &rd_set);

        ret = select(listen_fd + 1, &rd_set, NULL, NULL, NULL);

        if (ret < 0 && errno == EINTR)
        {
            continue;
        }

        if (ret < 0)
        {
            perror("select");
            exit(1);
        }

        for (int fd = 0; fd < listen_fd + 1; ++fd)
        {
            if (FD_ISSET(fd, &rd_set))
            {
                // 新的连接
                if (fd == listen_fd)
                {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
                    if (client_fd < 0)
                    {
                        perror("accept");
                        continue;
                    }

                    net_fd = client_fd;
                    break;
                }
            }
        }

        if (net_fd > 0)
        {
            break;
        }
    }

    return net_fd;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("tun [-c|-s] [tun_name] [addr] [port]\n");
        return 0;
    }

    bool client_flag = true;
    char *tun_name = NULL;
    char *addr = NULL;
    int port = 0;

    if (strcmp(argv[1], "-c") == 0)
    {
        client_flag = true;
        tun_name = argv[2];
        addr = argv[3];
        port = atoi(argv[4]);
        printf("Start client tun_name %s addr %s port %d\n", tun_name, addr, port);
    }

    if (strcmp(argv[1], "-s") == 0)
    {
        client_flag = false;
        tun_name = argv[2];
        addr = argv[3];
        port = atoi(argv[4]);
        printf("Start server tun_name %s addr %s port %d\n", tun_name, addr, port);
    }

    // create tun:
    int tun_fd;
    int nread;
    unsigned char buffer[2000];

    tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI);

    if (tun_fd < 0)
    {
        perror("Allocating interface.");
        exit(1);
    }

    // create socket
    int net_fd = client_flag == true ? createClientSocket(addr, port) : createServerSocket(addr, port);

    printf("tun_fd = %d, net_fd = %d\n", tun_fd, net_fd);

    int ret = 0;
    int max_fd = tun_fd > net_fd ? tun_fd : net_fd;
    while (1)
    {
        fd_set rd_set;

        FD_ZERO(&rd_set);
        FD_SET(tun_fd, &rd_set);
        FD_SET(net_fd, &rd_set);

        ret = select(max_fd + 1, &rd_set, NULL, NULL, NULL);

        if (ret < 0 && errno == EINTR)
        {
            continue;
        }

        if (ret < 0)
        {
            perror("select");
            exit(1);
        }

        int nread = 0;
        int nwrite = 0;
        unsigned short plength = 0;

        if (FD_ISSET(tun_fd, &rd_set))
        {
            nread = read(tun_fd, buffer, BUFSIZ);

            plength = htons(nread);
            nwrite = write(net_fd, (char *)&plength, sizeof(plength));
            nwrite = write(net_fd, buffer, nread);

            printf("Read %d bytes from device %s\n", nread, tun_name);
        }

        if (FD_ISSET(net_fd, &rd_set))
        {
            nread = read(net_fd, (char *)&plength, sizeof(plength));
            nread = read(net_fd, buffer, ntohs(plength));

            nwrite = write(tun_fd, buffer, nread);

            printf("Write %d bytes to device %s\n", nread, tun_name);
        }
    }

    return 0;
}
