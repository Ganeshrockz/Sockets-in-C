#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#define LISTENERS	10
#define MSGSIZE		250

void get_input(char* message)
{
	printf("=> ");
	scanf("%[^$]", message);
	while(message[strlen(message)-1]==10)
		message[strlen(message)-1] = 0;
	getchar(); getchar();
}

void put_output(char* message)
{       
        printf("\nWall: %s\n", message);
}

void* send_msg(void* arg)
{
	int client = *(int*) arg;
	char message[MSGSIZE];
	while(1) {
		memset(message, 0, MSGSIZE);
		get_input(message);
		if(!strcmp(message, "/quit"))
                        exit(0);
		send(client, message, strlen(message), 0);
	}
}

void* recv_msg(void* arg)
{
	int client = *(int*) arg;
	char message[MSGSIZE];
        while(1) {
		memset(message, 0, MSGSIZE);
                recv(client, &message, MSGSIZE, 0);
		if(strcmp(message, "\0")) put_output(message);
        }
}

int main(int number, char* port[])
{
	if(number != 2) {
		printf("Use like this %s <server port number>\n", port[0]);
		return 1;
	}

	unsigned short server_port = atoi(port[1]);
	if(server_port < 1000 || server_port > 2000) {
		printf("Please use Port number from 1000 to 2000\n");
		return 1;
	}
	printf("=> Port accepted\n");

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0) {
		printf("Socket is not created!\n");
		return 1;
	}
	printf("=> Socket created\n");

	struct sockaddr_in server;
	server.sin_family	= AF_INET;
	server.sin_port 	= htons(server_port); 
	server.sin_addr.s_addr	= INADDR_ANY;

	if(bind(server_socket, (struct sockaddr*) &server, sizeof server) < 0) {
		printf("Binding error\n");
		return 1;
	}
	printf("=> Binding to Port Number %u Successfull!\n", server_port);

	listen(server_socket, LISTENERS);
	printf("=> Listening to %d Listeners\n", LISTENERS);

	int client = accept(server_socket, NULL, NULL);
	if(client != -1) {
		printf("\n\nClient connected!\n\n");
		pthread_t s_tid, r_tid;
		pthread_create(&s_tid, NULL, send_msg, &client);
		pthread_create(&r_tid, NULL, recv_msg, &client);
		pthread_join(s_tid, NULL);
		pthread_join(r_tid, NULL);
		close(client);
		printf("\n\nClient closed\n\n");
	}
	close(server_socket);
	return 0;
}
