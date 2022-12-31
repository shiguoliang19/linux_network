#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "kcp/ikcp.h"

typedef struct kcp_user_data_
{
	int sockfd;

	struct sockaddr_in cli_addr;

	socklen_t cli_addr_len;

	struct sockaddr_in srv_addr;

	socklen_t srv_addr_len;

} kcp_user_data_t;

/* get system time */
static inline void itimeofday(long *sec, long *usec)
{
	#if defined(__unix)
	struct timeval time;
	gettimeofday(&time, NULL);
	if (sec) *sec = time.tv_sec;
	if (usec) *usec = time.tv_usec;
	#else
	static long mode = 0, addsec = 0;
	BOOL retval;
	static IINT64 freq = 1;
	IINT64 qpc;
	if (mode == 0) {
		retval = QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		freq = (freq == 0)? 1 : freq;
		retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
		addsec = (long)time(NULL);
		addsec = addsec - (long)((qpc / freq) & 0x7fffffff);
		mode = 1;
	}
	retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
	retval = retval * 2;
	if (sec) *sec = (long)(qpc / freq) + addsec;
	if (usec) *usec = (long)((qpc % freq) * 1000000 / freq);
	#endif
}

/* get clock in millisecond 64 */
static inline IINT64 iclock64(void)
{
	long s, u;
	IINT64 value;
	itimeofday(&s, &u);
	value = ((IINT64)s) * 1000 + (u / 1000);
	return value;
}

static inline IUINT32 iclock()
{
	return (IUINT32)(iclock64() & 0xfffffffful);
}

/* sleep in millisecond */
static inline void isleep(unsigned long millisecond)
{
	#ifdef __unix 	/* usleep( time * 1000 ); */
	struct timespec ts;
	ts.tv_sec = (time_t)(millisecond / 1000);
	ts.tv_nsec = (long)((millisecond % 1000) * 1000000);
	/*nanosleep(&ts, NULL);*/
	usleep((millisecond << 10) - (millisecond << 4) - (millisecond << 3));
	#elif defined(_WIN32)
	Sleep(millisecond);
	#endif
}

int udpOutput(const char* buf, int len, ikcpcb* kcp, void* user)
{
	kcp_user_data_t* ku = (kcp_user_data_t*)user;
	char hex[BUFSIZ] = {};
	int retval = -1;

	for(int i = 0; i < len; ++i)
	{
		sprintf(hex + 3 * i, "%02x ", (unsigned char)buf[i]);
	}
	hex[3 * len + 1] = '\0';
	printf("udpOutput: buf = %s len = %d\n", hex, len);

	retval = sendto(ku->sockfd, buf, len, MSG_DONTWAIT, (struct sockaddr*)&ku->srv_addr, ku->srv_addr_len);
	if(retval < 0)
	{
		perror("sendto");
		return -1;
	}

	return retval;
}

int main()
{
	int sockfd = -1;
	ikcpcb* kcp = NULL;
	char* msg = "ping";
	char buf[BUFSIZ] = {};
	int len = -1;
	kcp_user_data_t ku = {};
	int retval = -1;
	int size = -1;
	char data[BUFSIZ] = {};
	struct sockaddr_in cli_addr = {};
	socklen_t cli_addr_len = sizeof(cli_addr);
	struct sockaddr_in srv_addr = {};
	socklen_t srv_addr_len = sizeof(srv_addr);

	// socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	// srv addr
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	srv_addr.sin_port = htons(8080);

	ku.sockfd = sockfd;
	ku.srv_addr = srv_addr;
	ku.srv_addr_len = srv_addr_len;

	// ikcp create
	kcp = ikcp_create(0x01, (void*)&ku);
	kcp->output = udpOutput;
	ikcp_nodelay(kcp, 0, 10, 0, 0);
	ikcp_wndsize(kcp, 128, 128);

	// send
	len = strlen(msg);
	retval = ikcp_send(kcp, msg, len);
	if(retval < 0)
	{
		perror("ikcp_send: error!\n");
		return -1;
	}
	printf("ikcp_send: [%s] len = %d\n", msg, len);

	while (1)
	{
		isleep(10);

		ikcp_update(kcp, iclock());

		// recv
		len = recvfrom(sockfd, buf, BUFSIZ, MSG_DONTWAIT, NULL, NULL);
		if(len < 0)
		{
			continue;
		}

		retval = ikcp_input(kcp, buf, len);
		if(retval < 0)
		{
			continue;
		}

		size = 0;
		while (1)
		{
			len = ikcp_recv(kcp, data, BUFSIZ);
			if(len < 0)
			{
				break;
			}
			else
			{
				size = len;
				data[size] = '\0';
			}
		}

		if(size != 0)
		{
			printf("recv: [%s] len: %d\n", data, size);
		}
	}

	return 0;
}
