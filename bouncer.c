/* Port Bouncer
* To be called as nbouncer local_ip local_port remote_ip remote_port
*/

#include "bouncer.h"


void process_pkt(u_char *args, const struct pcap_pkthdr *header,const u_char *packet);



int main(int argc, char *argv[])
{
	count=0;
	printf("##########################################################################\n");
	printf("	                     Bouncer\n");
	printf("##########################################################################\n");
	dev = argv[1];
	listenip = argv[2];
	ip1 = atoi(strtok(listenip,"."));
	ip2 = atoi(strtok(NULL,"."));
	ip3 = atoi(strtok(NULL,"."));
	ip4 = atoi(strtok(NULL,"."));
	sprintf(listenip,"%d.%d.%d.%d",ip1,ip2,ip3,ip4);
	listenport = argv[3];//atoi
	serverip = argv[4];
	serverport = argv[5];//atoi
	incport=8000;
	printf("Interface: %s\nListen IP: %s\nListen Port: %s\nServer IP: %s\nServer Port: %s\n",dev,listenip,listenport,serverip,serverport);
	inet_aton(serverip,&server);
	inet_aton(listenip,&bouncerinet);

		// promiscuous mode : snap all packets
		pcap = pcap_open_live(dev, BUFSIZ, 1, 1000, error);
		if (pcap == NULL) {
			printf("%s\n", error);
			return(-1);

		}


	/* Initialize raw socket */
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	
	int one = 1;
	const int *val = &one;
	if (setsockopt (sock, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
		printf ("Warning: Cannot set HDRINCL!\n");
	



	pcap_loop(pcap, 100000000, process_pkt, NULL);
	/* And close the session */
	pcap_close(pcap);




	return 0;
}//End of the bouncer
