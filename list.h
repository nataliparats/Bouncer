#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>


struct Node{  
      char* ip;
      u_int16_t id;
      struct Node *Next;  
}*Head;   

void add(char* ip,u_int16_t id);
char* del(u_int16_t id);


struct tcpNode{
      char* ip;
      u_short port;
      u_short portserverside;
      int state;
      struct tcpNode *Next;  
}*TCPHead;   

void setStatetcp(u_short port,u_short state);
void printlist();
void addtcp(char* ip, u_short port,u_short portserverside,int state);
char* deltcp(u_short port);
int getStatetcp(u_short port);
u_short getPortServerSide(u_short port);
u_short getPort(u_short portserverside);
char* getIP(u_short port);
