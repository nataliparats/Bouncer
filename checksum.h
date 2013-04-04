#define _BSD_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pcap.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>

struct tcp_pseudo /*the tcp pseudo header*/
{
  struct in_addr src_addr;
  struct in_addr dst_addr;
  u_char zero;
  u_char proto;
  u_short length;
} pseudohead;


long get_tcp_checksum(struct ip * myip, struct tcphdr * mytcp);
unsigned short Checksum(unsigned short *ptr, int len);
