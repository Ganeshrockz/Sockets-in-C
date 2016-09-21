#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(int Total_Args, char* Args[])
{
	if(Total_Args != 2) {
		printf("Did you mean?\n");
		printf("%s <port no.>\n", Args[0]);
		return 1;
	}

	unsigned short port = atoi(Args[1]);
	if(port<100 || port>10000) {
		printf("Use valid port\n");
		return -1;
	}

	int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in server;
	server.sin_family	= AF_INET;
	server.sin_port		= htons(port);
	server.sin_addr.s_addr	= INADDR_ANY;

	if(connect(server_socket, (struct sockaddr*)&server, sizeof server)!=0) {
		printf("Connection error\n");
		return -1;
	}	

	printf("Connected successfully\n");

	char echo[1024];

	printf("Client> ");
	scanf("%[^\n]", echo);
	getchar();
	int size;
	sendto(server_socket, echo, sizeof echo, 0, (struct sockaddr*)&server, size);
	memset(echo, 0, sizeof echo);
	recvfrom(server_socket, echo, 1024, 0, (struct sockaddr*)&server, &size);
	printf("Server: %s\n", echo);
	close(server_socket);
	return 0;
}
