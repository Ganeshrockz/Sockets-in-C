//Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     //Unix command
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE 1024

void* client_work(void* arg)
{
	int client = *(int *)arg;

	//Echo server
	char echo[SIZE];
	memset(echo, 0, sizeof echo);
	recv(client, echo, SIZE, 0);
	printf("Client: %s\n", echo);
	send(client, echo, strlen(echo), 0);

}

int main(int TotalArgs, char* Args[])
{
	//COMMON START
	int server_socket = socket(AF_INET, SOCK_STREAM, 0); //Process ID of the server socket

	if(TotalArgs != 2) {
		printf("use like this %s <port no.>\n", Args[0]);
		return -1;
	}

	unsigned short port = atoi(Args[1]);

	struct sockaddr_in server;
	server.sin_addr.s_addr  = INADDR_ANY; //0.0.0.0, 127.0.0.1, 192.168.X.X 
	server.sin_port         = htons(port);
	server.sin_family       = AF_INET;
	//COMMON END

	if(bind(server_socket, (struct sockaddr*)&server, sizeof server) != 0) {
		puts("Binding failed");
		return -1;
	}

	if(listen(server_socket, 10) != 0) {
		puts("Listening error");
		return -1;
	}

	puts("Server is active");

	while(1) {
		int client = accept(server_socket, NULL, NULL);

		client_work(&client);

		close(client);
	}

	close(server_socket);
	return 0;
}
