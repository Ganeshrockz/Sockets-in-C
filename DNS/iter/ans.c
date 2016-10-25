#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#define port 3053
int main(int argc, char *argv[])
{
        struct sockaddr_in server;
        int sd1;
        sd1=socket(AF_INET,SOCK_DGRAM,0);
        char host[100],addr[100];
        char hn[10][20]={"google","yahoo","facebook","gmail","hotmail","prasan","chronus","nutonix","amazon","samsung"};
        char ip[10][30]={"1.1.1.1","2.2.2.2","3.3.3.3","5.5.5.5","6.6.6.6","7.7.7.7","8.8.8.8","9.9.9.","10.10.10.10","11.11.11.11"};
        server.sin_family=AF_INET;
        server.sin_port=htons(atoi(argv[1]));
        server.sin_addr.s_addr=inet_addr("127.0.0.1");
        int n=sizeof(server);
        bind(sd1,(struct sockaddr *)&server,sizeof(server));
        while(1)
        {
        recvfrom(sd1,host,sizeof(host),0,(struct sockaddr *)&server,&n);
        int i;
        strcpy(addr,"error:404 not found");
        for(i=0;i<10;i++)
         if(strcmp(host,hn[i])==0)
                 {
                         strcpy(addr,ip[i]);
                         break;
                 }
        printf("\n Request reached ANS... Responded Back");
        sendto(sd1,addr,sizeof(addr),0,(struct sockaddr *)&server,n);
        }
        return 0;
}