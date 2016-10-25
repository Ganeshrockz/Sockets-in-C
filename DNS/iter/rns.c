#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
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
        struct sockaddr_in server,ans;
        int sd1,sd2;
        sd1=socket(AF_INET,SOCK_DGRAM,0);
        char host[100],addr[100];
        char hn[5][20]={"gmail","hotmail","prasan","x"};
        char ip[3][30]={"5.5.5.5","6.6.6.6","7.7.7.7"};
        server.sin_family=AF_INET;
        server.sin_port=htons(atoi(argv[1]));
        server.sin_addr.s_addr=inet_addr("127.0.0.1");
        int n=sizeof(server);
        bind(sd1,(struct sockaddr *)&server,sizeof(server));
        while(1)
        {
        recvfrom(sd1,host,sizeof(host),0,(struct sockaddr *)&server,&n);
        int i;
        strcpy(addr,"");
        for(i=0;i<5;i++)
         if(strcmp(host,hn[i])==0)
                 {
                         printf("\n response from rns");
                         fflush(stdout);
                         strcpy(addr,ip[i]);
                         break;
                 }
        if(strcmp(addr,"")==0)
        {
 printf("\nHost Name not found in rns...");
                 strcpy(addr,"NOT FOUND");
//               break;

        }
        sendto(sd1,addr,sizeof(addr),0,(struct sockaddr *)&server,n);
        }
        return 0;
}
