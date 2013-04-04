#include "list.h"


void add(char* ip,u_int16_t id){  
   struct Node *temp;  
   temp=(struct Node *)malloc(sizeof(struct Node));  
   temp->ip=malloc(64);
   strcpy(temp->ip,ip);
   temp->id = id;  
  
   if (Head == NULL){  
      Head=temp;  
      Head->Next=NULL;  
   }  
   else{  
      temp->Next=Head;  
      Head=temp;  
   }  
}  


char* del(u_int16_t id){  
   struct Node *prev_ptr, *cur_ptr;    
   cur_ptr=Head; 
   char* res=NULL;
  
   while(cur_ptr != NULL){  
      if(cur_ptr->id == id){  
         if(cur_ptr==Head){  
	    res=malloc(sizeof(cur_ptr->ip));
            strcpy(res,cur_ptr->ip);
            Head=cur_ptr->Next;  
            free(cur_ptr);  
            return res;  
         }  
         else{  
	    res=malloc(sizeof(cur_ptr->ip));
            strcpy(res,cur_ptr->ip);
            prev_ptr->Next=cur_ptr->Next;  
            free(cur_ptr);  
            return res;  
         }  
      }  
      else{  
         prev_ptr=cur_ptr;  
         cur_ptr=cur_ptr->Next;  
      }  
   }  
   return res;  
}  

void addtcp(char* ip,u_short port,u_short portserverside,int state){  
   struct tcpNode *temp;  
   temp=(struct tcpNode *)malloc(sizeof(struct tcpNode));  
   temp->ip=malloc(64);
   strcpy(temp->ip,ip);
   temp->port = port;
   temp->state = state;

   temp->portserverside= portserverside;
  
   if (TCPHead == NULL){  
      TCPHead=temp;  
      TCPHead->Next=NULL;  
   }  
   else{  
      temp->Next=TCPHead;  
      TCPHead=temp;  
   }  
}  



char* deltcp(u_short port){  
   struct tcpNode *prev_ptr, *cur_ptr;    
   cur_ptr=TCPHead; 
   char* res=NULL;
  
   while(cur_ptr != NULL){  
      if(cur_ptr->port == port){  
         if(cur_ptr==TCPHead){  
	    res=malloc(sizeof(cur_ptr->ip));
            strcpy(res,cur_ptr->ip);
            TCPHead=cur_ptr->Next;  
            free(cur_ptr);  
            return res;  
         }  
         else{  
	    res=malloc(sizeof(cur_ptr->ip));
            strcpy(res,cur_ptr->ip);
            prev_ptr->Next=cur_ptr->Next;  
            free(cur_ptr);  
            return res;  
         }  
      }  
      else{  
         prev_ptr=cur_ptr;  
         cur_ptr=cur_ptr->Next;  
      }  
   }  
   return res;  
}  

void printlist(){
   struct tcpNode *cur_ptr;    
   cur_ptr=TCPHead; 
 
   while(cur_ptr != NULL){  
      printf("ip:%s,port:%d,portss:%d\n",cur_ptr->ip,ntohs(cur_ptr->port),ntohs(cur_ptr->portserverside));
      cur_ptr=cur_ptr->Next;  
   }  

}



int getStatetcp(u_short port){  
   struct tcpNode *cur_ptr;    
   cur_ptr=TCPHead; 
   int res=-1;
  
   while(cur_ptr != NULL){  
      if(cur_ptr->port == port)  
	 return cur_ptr->state;
      cur_ptr=cur_ptr->Next;  
   }  
   return res;  
}  


void setStatetcp(u_short port,u_short state){  
   struct tcpNode *cur_ptr;    
   cur_ptr=TCPHead; 
  
   while(cur_ptr != NULL){  
      if(cur_ptr->port == port){
	 cur_ptr->state =state; 
	 return;}
      cur_ptr=cur_ptr->Next;  
   }  

}  



char* getIP(u_short port){  
   struct tcpNode *cur_ptr;    
   cur_ptr=TCPHead; 
   char* res=NULL;
  
   while(cur_ptr != NULL){  
      if(cur_ptr->port == port){  
	    res=malloc(sizeof(cur_ptr->ip));
            strcpy(res,cur_ptr->ip);
            return res;  
      }  
      cur_ptr=cur_ptr->Next;  
   }  
   return res;  
}  







u_short getPort(u_short portserverside){  
   struct tcpNode *cur_ptr;    
   cur_ptr=TCPHead; 
   while(cur_ptr != NULL){  
      if(cur_ptr->portserverside == portserverside)  
	    return cur_ptr->port;
      cur_ptr=cur_ptr->Next;  
   }  
   printf("not found\n");
   return 0;  
}


u_short getPortServerSide(u_short port){  
   struct tcpNode *cur_ptr;    
   cur_ptr=TCPHead; 
   while(cur_ptr != NULL){  
      if(cur_ptr->port == port)  
	    return cur_ptr->portserverside;
      cur_ptr=cur_ptr->Next;  
   }  
   printf("not found\n");
   return 0;  
}    
