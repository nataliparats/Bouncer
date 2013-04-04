#include "bouncer.h"


int offsetsize(struct ip* ip,char* listenip){
	int res=0;
	res=strlen(listenip)-strlen(inet_ntoa(ip->ip_src));	
	return res;
	
}

void process_pkt(u_char *args, const struct pcap_pkthdr *header,
	    const u_char *p){
	/* Define pointers for packet's attributes */
	struct ether_header* eth = (struct ether_header*)(p);
	struct icmphdr* icmp = NULL;
	struct ip* ip;
	struct tcphdr* tcp;
	u_short checksum,checksum2;
	char* dest=NULL;
	char* dport=NULL;
	char* sport=NULL;


	if (ntohs(eth->ether_type)==ETHERTYPE_IP){

		/* Check IP header*/
		ip = (struct ip*)( p + sizeof(struct ether_header));
		checksum =  ip->ip_sum;	
		int size_ip = ip->ip_hl*4;
		if (strcmp(inet_ntoa(ip->ip_dst),listenip)!=0){
			return;		
		}
		if (ip->ip_v!=IPVERSION){
			printf("IP: error version!=4\n");
			return;
		}
		if (ip->ip_hl*4<20){	
			printf("IP: header length <20\n");
			return;
		}
		if (ip->ip_hl*4>60){	
			printf("IP: header length >60\n");
			return;
		}
		ip->ip_sum=0;
		checksum2 = Checksum((unsigned short *)ip,size_ip);
		ip->ip_sum=checksum;
		if (checksum!=checksum2){
			printf("IP: error in the checksum\n");
			return;
		}	
		if (ntohs(ip->ip_off)==IP_RF){
			printf("IP: EVIL BIT\n");
			return;
		}		
		if (ip->ip_ttl==0){	
			printf("IP: TTL=0\n");
			return;
		}
		if (ip->ip_ttl>MAXTTL){	
			printf("IP: TTL=MAXTTL\n");
			return;
		}


		/* Check type of packet and process*/



		switch(ip->ip_p) {
			case IPPROTO_TCP:
				tcp =(struct tcphdr*)(p +sizeof(struct ether_header)+size_ip);
				dport=malloc(64);
				sprintf(dport,"%u",ntohs(tcp->th_dport));
				sport=malloc(64);
				sprintf(sport,"%u",ntohs(tcp->th_sport));
				if (tcp->th_off*4<20){	
					printf("TCP: header length <20\n");
					return;
				}


				if (strcmp(dport,listenport)!=0){
					if (getPort(tcp->th_dport)==0){
						printf("wrongth port %s!=%s \n",dport,listenport);
						return;		
					}
				}

	
				checksum = tcp->th_sum;
				tcp->th_sum=0;
				checksum2 = get_tcp_checksum(ip,tcp) ;
				tcp->th_sum=  checksum2 ;

				if (checksum!=checksum2){
					printf("TCP: error checksum: %d!=%d\n",checksum2,checksum);
					return;
				}

				//FTP
				if (atoi(listenport)==21){
					
					data=(char *)p+sizeof(struct ether_header)+tcp->th_off*4+sizeof(struct ip);

					if (strstr(data,"PORT")==data)
					{
						strtok(data,",");
						strtok(NULL,",");
						strtok(NULL,",");
						strtok(NULL,",");
						results=strtok(NULL,",");
						results2=strtok(NULL,",");
						portline=malloc(64);
						ftpport=atoi(results)*256+atoi(results2);
						sprintf(portline,"PORT %d,%d,%d,%d,%s,%s\r\n",ip1,ip2,ip3,ip4,results,results2);
						strcpy(data,portline);
						addtcp(inet_ntoa(ip->ip_src),htons(ftpport),htons(ftpport),666);
						ip->ip_len=htons(ntohs(ip->ip_len)+offsetsize(ip,listenip));
					}	

				}


				if(getStatetcp(tcp->th_dport)==-1 && tcp->th_flags==TH_SYN){
					incport++;
					addtcp(inet_ntoa(ip->ip_src),tcp->th_sport,htons(incport),0);
				}

	
				if (atoi(listenport)==21 && getStatetcp(tcp->th_dport)==666 && tcp->th_flags==TH_SYN && strcmp(inet_ntoa(ip->ip_dst),listenip)==0){
					setStatetcp(tcp->th_dport,ntohs(tcp->th_sport));
				}



				if (strcmp(inet_ntoa(ip->ip_src),serverip)==0){ 
					printf("tcp:server--->bouncer\n");
					dest= getIP(getPort(tcp->th_dport));
					if (dest==NULL){
						printf("error port\n"); 
						return;
					}
					inet_aton(dest,&(ip->ip_dst));
					sockinetadd.sin_family = AF_INET;
					sockinetadd.sin_addr.s_addr = inet_addr(dest);
					sockinetadd.sin_port = htons(getPort(tcp->th_dport));
					//PORT
					tcp->th_dport=getPort(tcp->th_dport);
					tcp->th_sport=htons(atoi(listenport));
				
				}
				else {
					if (getStatetcp(tcp->th_sport)==-1){
						printf("not in the list drop\n");
						return;
					}
					printf("tcp: client----->bouncer\n");
					ip->ip_dst=server;
					sockinetadd.sin_family = AF_INET;
					sockinetadd.sin_port = htons(atoi(serverport));
					sockinetadd.sin_addr.s_addr = inet_addr(serverip);
					//PORT
					if (getStatetcp(tcp->th_sport)>0)
						tcp->th_dport=htons(20);
					else
						tcp->th_dport=htons(atoi(serverport));
					tcp->th_sport=getPortServerSide(tcp->th_sport);

				}
				inet_aton(listenip,&(ip->ip_src));


				tcp->th_sum=htons(0);
				checksum2 = get_tcp_checksum(ip,tcp);
				tcp->th_sum=  checksum2;
			  	count++;
				//ip->ip_len=htons(sizeof(struct ip)+sizeof(struct icmphdr))*8;

				ip->ip_sum=0;
				checksum2 = Checksum((unsigned short *)ip,size_ip);
				ip->ip_sum = checksum;

				//send the packet:
				if(sendto(sock, ip, ntohs(ip->ip_len), 0, (struct sockaddr *)&sockinetadd, sizeof(sockinetadd)) <= 0){
					perror("sendto");
					return;
				}



				
				


				break;
			case IPPROTO_UDP:
				printf("UDP\n");
				return;
				break;
			case IPPROTO_ICMP:
				/* Check ICMP header*/
				icmp =(struct icmphdr*)(p +sizeof(struct ether_header)+size_ip);
				checksum = icmp->checksum;
				icmp->checksum = 0;
				checksum2 = Checksum((unsigned short *)icmp,sizeof(struct icmphdr)*8);
				icmp->checksum= Checksum((unsigned short *)icmp,sizeof(struct icmphdr)*8);

				if (checksum!=checksum2){
					printf("ICMP: error in the checksum: %d!=%d\n",checksum2,checksum);
					return;
				}	
				if (icmp->type==ICMP_ECHOREPLY && icmp->code==0){
					printf("ICMP:Reply\n");
					dest=del(icmp->un.echo.id);
					if (dest==NULL){
						printf("|error id\n"); 
						return;
					}
					inet_aton(dest,&(ip->ip_dst));
					sockinetadd.sin_family = AF_INET;
					sockinetadd.sin_addr.s_addr = inet_addr(dest);
					
				}
				else if	(icmp->type==ICMP_ECHO && icmp->code==0){
					printf("ICMP: Echo Request\n");
					add(inet_ntoa(ip->ip_src),icmp->un.echo.id);
					ip->ip_dst=server;
					sockinetadd.sin_family = AF_INET;
					sockinetadd.sin_addr.s_addr = inet_addr(serverip);
				}
				else {
					printf("ICMP: error code or type\n");
					return;
				}	
				
				ip->ip_len=htons(sizeof(struct ip)+sizeof(struct icmphdr))*8;
				inet_aton(listenip,&(ip->ip_src));
				ip->ip_sum=0;
				checksum2 = Checksum((unsigned short *)ip,size_ip);
				ip->ip_sum = checksum;


				//send the packet:
				if(sendto(sock, ip, ntohs(ip->ip_len), 0, (struct sockaddr *)&sockinetadd, sizeof(sockinetadd)) <= 0){
					perror("sendto");
					return;
				}
				break;

				

			case IPPROTO_IP:
				printf("IP\n");
				return;
				break;
			default:
				printf("IP proto :unknown\n");
				return;
				break;
		}


		
		free(dport);
		free(sport);

		
		


	}






}
	
