#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
int main()
{
	int sock,bytes_received,i=1;
	char receive[30];
	struct hostent *host;
	struct sockaddr_in server_addr;
	host=gethostbyname("127.0.0.1");
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("Socket not created");
		exit(1);
	}
	printf("Socket created\n");
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(17000);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);
	if(connect(sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))==-1)
	{
		perror("Connect");
		exit(1);
	}
	while(1)
	{
		memset(receive, 0, sizeof receive);
		recv(sock, receive, 25, 0);
		if(strcmp(receive,"exit")==0)
		{
			close(sock);
			break;
		}
		else
		{
			if(strlen(receive)<=10)
			{
				printf("Frame %d data %s received\n", i, receive);
				send(sock, receive, strlen(receive), 0);
			}
			else
			{
				send(sock,"negative", strlen("negative"), 0);
			}
			i++;
		}
	}
	close(sock);
	return(0);
}
