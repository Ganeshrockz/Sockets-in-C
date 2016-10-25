#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
int main(int argc, char *argv[])
{
	int sd;
	char host[100],ip[100];
	struct sockaddr_in server, rns, ans;
	memset(&server,0,sizeof(server));
	sd=socket(AF_INET,SOCK_DGRAM,0);
	if(sd==-1)
	{
		printf("\n error in sockte creation");
		exit(0);
	}
	printf("\n socket created successfully");
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(atoi(argv[1]));
	while(1)
	{
		printf("\n Enter HostName :");
		scanf("%s",host);
		int n=sizeof(server);
		sendto(sd,host,sizeof(host),0,(struct sockaddr *)&server,n);
		printf("\n passing hostname to local name server...\n");
		recvfrom(sd,ip,sizeof(ip),0,(struct sockaddr *)&server,&n);
		//printf("response %s", ip);
		if(strcmp(ip, "NOT FOUND") == 0){
			printf("response %s", ip);
			printf("\n\n\nTrying to fetch from rns....");
			rns.sin_family=AF_INET;
			rns.sin_addr.s_addr=inet_addr("127.0.0.1");
			rns.sin_port=htons(atoi(argv[2]));
			sendto(sd,host,sizeof(host),0,(struct sockaddr *)&rns,n);
			printf("\n passing hostname to rns...\n");
			recvfrom(sd,ip,sizeof(ip),0,(struct sockaddr *)&rns,&n);

			if(strcmp(ip, "NOT FOUND") == 0){
				printf("response %s", ip);
				printf("\n\n\nTrying to fetch from ANS....");
				ans.sin_family=AF_INET;
				ans.sin_addr.s_addr=inet_addr("127.0.0.1");
				ans.sin_port=htons(atoi(argv[3]));
				sendto(sd,host,sizeof(host),0,(struct sockaddr *)&ans,n);
				printf("\n passing hostname to ans...\n");
				recvfrom(sd,ip,sizeof(ip),0,(struct sockaddr *)&ans,&n);
			}
		}
		printf("%s is its ip",ip);
	}
	return 0;
}
