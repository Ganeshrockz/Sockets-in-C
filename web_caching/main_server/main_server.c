#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#define LISTENERS	10
#define BUFFSIZE	1024
#define FILESIZE	128

int sendRes(int client, char* filename, char* response, int status)
{
	char data[BUFFSIZE] = "\0";
	if(status == 0) {
		strcpy(data, "<html><head><title>Error</title></head><body><h1>404 Error</h1></body></html>");
	} else {
		FILE *FP = fopen(filename, "r");
		fscanf(FP, "%[^EOF]", data);
		fclose(FP);
	}
	strcat(response, data);
	send(client, response, strlen(response), 0);
	return 0;
}

int recvReq(int client, char* filename)
{
	char request[BUFFSIZE] = "\0";
	recv(client, request, sizeof request, 0);
	puts(request);

	if(request[0]=='G' && request[1]=='E' && request[2]=='T') {
		if(request[5] == ' ') {
			strcpy(filename, "index.html");
		} else {
			int i = 5;
			while(request[i]!=' ') {
				filename[i-5] = request[i];
				i++;
			}
			filename[i-5] = 0;
		}	
	}
	return 0;
}	

int check_file(char* filename, char* response)
{
	if(access(filename, 0) == 0) {
		strcpy(response, "HTTP/1.1 GET OK\r\n\r\n");
		return 1;
	}
	else {
		strcpy(response, "HTTP/1.1 404 File not found\r\n\r\n");
		return 0;
	}
}

void* client_work(void* arg)
{
	int* client = (int*) arg;

	char filename[FILESIZE];
	recvReq(*client, filename);

	char response[BUFFSIZE];
	int status = check_file(filename, response);

	sendRes(*client, filename, response, status);
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

	while(1) {
		int client = accept(server_socket, NULL, NULL);
		if(client != -1) {
			printf("Client connected!\n\n");
			pthread_t tid;
			pthread_create(&tid, NULL, client_work, &client);
			pthread_join(tid, NULL);
			close(client);
			printf("Client closed\n\n");
		}
	}
	close(server_socket);	
	return 0;
}
