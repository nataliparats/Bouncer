#include "checksum.h"

unsigned short Checksum(unsigned short *ptr, int len){
	register int sum = 0;
	u_short answer = 0;
	register u_short *w = ptr;
	register int nleft = len;


	while(nleft > 1){
	sum += *w++;
	nleft -= 2;
	}
	if (nleft==1){
		*((u_char *) &answer) = *(u_char *)w;
		sum+=answer;
	}
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	answer = ~sum;
	return(answer);
}



long get_tcp_checksum(struct ip * myip, struct tcphdr * mytcp) {

        int total_len = ntohs(myip->ip_len);

        int tcpopt_len = mytcp->th_off*4 - 20;
        int tcpdatalen = total_len - (mytcp->th_off*4) - (myip->ip_hl*4);

        pseudohead.src_addr=myip->ip_src;
        pseudohead.dst_addr=myip->ip_dst;
        pseudohead.zero=htons(0);
        pseudohead.proto=IPPROTO_TCP;
        pseudohead.length=htons(sizeof(struct tcphdr) + tcpopt_len + tcpdatalen);

        int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr) + tcpopt_len + tcpdatalen;
        unsigned short * tcp=malloc(totaltcp_len*sizeof(unsigned short));


        memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)mytcp,sizeof(struct tcphdr));
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo)+sizeof(struct tcphdr), (unsigned char *)myip+(myip->ip_hl*4)+(sizeof(struct tcphdr)), tcpopt_len);
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo)+sizeof(struct tcphdr)+tcpopt_len, (unsigned char *)mytcp+(mytcp->th_off*4), tcpdatalen);

         return Checksum(tcp,totaltcp_len);

}

