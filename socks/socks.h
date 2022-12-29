#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

#define BUFFER_SIZE (4096 * 10)

/*
    |client                 |proxy                  |server
    |          auth         |                       |
    |---------------------->|                       |
    |<----------------------|                       |
    |         proxy         |                       |
    |---------------------->|                       |
    |                       |---------------------->|
    |                       |<----------------------|
    |<----------------------|                       |
    |         data          |                       |
    |---------------------->|                       |
    |                       |---------------------->|
    |                       |<----------------------|
    |<----------------------|                       |
    |                       |                       |
*/

/*
    +----+----------+----------+
    |VER | NMETHODS | METHODS  |
    +----+----------+----------+
    | 1  |    1     | 1 to 255 |
    +----+----------+----------+
 */

typedef struct auth_request_ {
    char ver; /* 版本 0x05 */
    char n_methods; /* 认证方法数量 1～255 */
    char methods[255]; /* 认证方法 */
} auth_request_t;

/*
    +----+--------+
    |VER | METHOD |
    +----+--------+
    | 1  |   1    |
    +----+--------+
*/

typedef struct auth_reply_ {
    char ver; /* 版本 0x05 */
    char method; /* 认证方法 */
} auth_reply_t;

/*
    +----+-----+-------+------+----------+----------+
    |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
    +----+-----+-------+------+----------+----------+
    | 1  |  1  | X'00' |  1   | Variable |    2     |
    +----+-----+-------+------+----------+----------+
*/

typedef struct proxy_request_ {
    char ver; /* 版本 0x05 */
    char cmd; /* 命令 0x01 connect 0x02 bind 0x03 udp */
    char rsv; /* 保留字段 0x00 */
    char atyp; /* 类型 0x01 ipv4 0x02 domain name 0x03 ipv6 */
    struct in_addr dst_addr; /* 目标地址 */
    unsigned short dst_port; /* 目标端口 */
} proxy_request_t;

/*
    +----+-----+-------+------+----------+----------+
    |VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
    +----+-----+-------+------+----------+----------+
    | 1  |  1  | X'00' |  1   | Variable |    2     |
    +----+-----+-------+------+----------+----------+
*/

typedef struct proxy_reply_ {
    char ver; /* 版本 0x05 */
    char rep; /* 回复 0x00 成功 0x01 一般性失败 */
    char rsv; /* 保留字段 0x00 */
    char atyp; /* 类型 0x01 ipv4 0x02 domain name 0x03 ipv6 */
    struct in_addr bnd_addr; /* 地址 */
    unsigned short bnd_port; /* 端口 */
} proxy_reply_t;

typedef struct socket_pair_t {
    int fd;
    int peer_fd;
} socket_pair_t;

typedef enum sock5_process_ {
    SOCK5_LISTEN, /* 监听阶段 */
    SOCK5_AUTH, /* 认证阶段 */
    SOCK5_PROXY, /* 代理阶段 */
    SOCK5_DATA, /* 数据传输阶段 */
} sock5_process_t;

typedef enum socket_type_ {
    SOCKET_LISTEN, /* 监听类型 */
    SOCKET_PROXY, /* 代理类型 */
    SOCKET_SERVER, /* 服务器类型 */
} socket_type_t;

typedef struct sock5_proxy_ {
    socket_type_t socket_type; /* socket类型 */
    sock5_process_t sock5_process; /* sock5阶段 */
    socket_pair_t socket_pair; /* 套接字 */

    char* wbuffer; // 写入数据缓冲区
    int len; // 写入数据缓冲区大小
} sock5_proxy_t;

// 认证处理函数
void auth_handle_fn(const auth_request_t *request, auth_reply_t *reply);

// 代理处理函数
void proxy_handle_fn(const proxy_request_t *request, proxy_reply_t *reply, int epfd, sock5_proxy_t* sock5_proxy);
