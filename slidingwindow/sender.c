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
	int sock,bytes_received,connected,true=1,i=1,s,f=0,sin_size;
	char send_data[1024],data[1024],c,fr[30]=" ";
	struct sockaddr_in server_addr,client_addr;
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("Socket not created");
		exit(1);
	}
	if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int))==-1)
	{
		perror("Setsockopt");
		exit(1);
	}
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(17000);
	server_addr.sin_addr.s_addr=INADDR_ANY;
	if(bind(sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))==-1)
	{
		perror("Unable to bind");
		exit(1);
	}
	if(listen(sock,5)==-1)
	{
		perror("Listen");
		exit(1);
	}
	sin_size=sizeof(struct sockaddr_in);
	connected=accept(sock,(struct sockaddr *)&client_addr,&sin_size);
	while(strcmp(fr,"exit")!=0)
	{
		printf("Enter Data Frame %d:(Enter exit for End): ",i);
		scanf("%[^\n]",fr);
		getchar();
		send(connected,fr,strlen(fr),0);
		memset(data, 0, sizeof data);
		recv(connected, data, 1024, 0);
		if(strlen(data)!=0)
			printf("I got an acknowledgement : %s\n", data);
		i++;
	}
	close(sock);
	return (0);
}
