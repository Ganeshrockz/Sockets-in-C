#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
int main(int argc, char *argv[])
{
        printf("\nRunning in port %d",atoi(argv[1]));
        int sd1,sd2;
        char hn[5][20]={"google","yahoo","facebook"};
        char ip[5][30]={"1.1.1.1","2.2.2.2","3.3.3.3"};
        char host[100],addr[100];
        struct sockaddr_in server,rns;
        memset(&server,0,sizeof(server));
        sd1=socket(AF_INET,SOCK_DGRAM,0);
        if(sd1==-1)
        {
                printf("\n error in socket creation");
                exit(0);
        }
        printf("\n socket created successfully");
        server.sin_family=AF_INET;
        server.sin_addr.s_addr=inet_addr("127.0.0.1");
        server.sin_port=htons(atoi(argv[1]));
        bind(sd1,(struct sockaddr *)&server,sizeof(server));
        int n=sizeof(server);
        while(1){
                recvfrom(sd1,host,sizeof(host),0,(struct sockaddr *)&server,&n);
                int i;
                strcpy(addr,"");
                for(i=0;i<3;i++)
                        if(strcmp(host,hn[i])==0)
                        {
                                printf("\n response from lns");
                                fflush(stdout);
                                strcpy(addr,ip[i]);
                                break;
                        }
                if(strcmp(addr,"")==0)
                {
 printf("\nHost Name not found in lns...");
                        strcpy(addr,"NOT FOUND");
//                      break;
                }
                sendto(sd1,addr,sizeof(addr),0,(struct sockaddr *)&server,n);
        }
        return 0;
}
