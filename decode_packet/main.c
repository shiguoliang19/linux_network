#include <stdio.h>

#include <linux/types.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include <netinet/in.h>

#define MAC_ARG(p) p[0],p[1],p[2],p[3],p[4],p[5]

#define IP_ARG(p)  p[0],p[1],p[2],p[3]

unsigned char pkt[] = {
    0x30, 0xa1, 0xfa, 0x67, 0x40, 0x1c, 0x3c, 0x22, 0xfb, 0xa2, 0x5e, 0x93, 0x08, 0x00, 0x45, 0x00,
    0x00, 0x28, 0x00, 0x00, 0x40, 0x00, 0x40, 0x06, 0x18, 0x89, 0xc0, 0xa8, 0x65, 0x0f, 0x0d, 0x45,
    0xef, 0x4a, 0xdc, 0xd2, 0x01, 0xbb, 0x07, 0x76, 0xf5, 0x81, 0xcd, 0xfb, 0x3f, 0x02, 0x50, 0x10,
    0x10, 0x00, 0x95, 0x09, 0x00, 0x00
};

int main(int argc, char* argv)
{
    /* mac */
    struct ethhdr *ethh;
    struct iphdr *iph;
    struct tcphdr *tcph;
	unsigned char *p = pkt;
	unsigned char *q;
	
	ethh = (struct ethhdr *)p;

	printf("h_dest:%02x:%02x:%02x:%02x:%02x:%02x \n", MAC_ARG(ethh->h_dest));
	printf("h_source:%02x:%02x:%02x:%02x:%02x:%02x \n", MAC_ARG(ethh->h_source));
	printf("h_proto:%04x\n",ntohs(ethh->h_proto));

    /* ip */
    if(ntohs(ethh->h_proto) == 0x0800)
	{
		iph = (struct iphdr *)(p + sizeof(struct ethhdr));

		q = (unsigned char *)&(iph->saddr);
		printf("src ip:%d.%d.%d.%d\n",IP_ARG(q));

		q = (unsigned char *)&(iph->daddr);
		printf("dest ip:%d.%d.%d.%d\n",IP_ARG(q));
	}

    /* tcp */
    switch(iph->protocol)
    {
        case 0x1:
            //icmp
            break;
        case 0x6:
            //tcp				
            tcph = (struct tcphdr *)(p + sizeof(struct ethhdr) + sizeof(struct iphdr));
            printf("source:%d dest:%d \n",ntohs(tcph->source),ntohs(tcph->dest));	
            break;
        case 0x11:
            //udp
            
            break;
    }

    return 0;
}