#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE 2048

int main(int TotalArgs, char* Args[])
{
	//COMMON BEGIN
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(TotalArgs != 2) {
		printf("use like this %s <port>\n", Args[0]);
		return -1;
	}
	unsigned short port = atoi(Args[1]);

	struct sockaddr_in server;
	server.sin_addr.s_addr	= INADDR_ANY; 
	server.sin_family	= AF_INET;
	server.sin_port		= htons(port);
	//COMMON END

	if(connect(server_socket, (struct sockaddr*)&server, sizeof server) != 0) {
		puts("Connection error");
		return -1;
	}

	char req[SIZE];
	char res[SIZE];
	char filename[SIZE];

	strcpy(req, "GET ");
	printf("Enter the filename: ");
	scanf("%s", filename);
	strcat(req, filename);
	strcat(req, " HTTP/1.1\r\n\n");
	puts(req);
	send(server_socket, req, strlen(req), 0);

	do {
		memset(res, 0, sizeof res);
		recv(server_socket, res, SIZE, 0);
		puts(res);
	}while(strcmp(res, "") != 0);
	close(server_socket);
	return 0;
}
