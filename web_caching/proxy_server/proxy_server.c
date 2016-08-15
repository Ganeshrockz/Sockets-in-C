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
#define FILENO 		10

struct files {
	char filename[FILESIZE];
	int timestamp;
}Files[FILENO];

int NumberOfFiles = 0;
unsigned short server_port;

int isPresent(char* filename)
{
	int i;
	for(i=0; i<NumberOfFiles; ++i) {
		if(strcmp(filename, Files[i].filename) == 0) {
			return 1;
		}
	}
	return 0;
}

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

void create_update(char* response, char* filename)
{
	if(response[9] == 'G') {	
		char temp[BUFFSIZE];
		int i;
		for(i=19; response[i]; ++i)
			temp[i-19] = response[i];
		temp[i-19] = 0;
		FILE* FP = fopen(filename, "w");
		fprintf(FP, "%s", temp);
		fclose(FP);
		strcpy(Files[NumberOfFiles].filename, filename);
		NumberOfFiles = (NumberOfFiles + 1)%FILENO;
	}
}

void* client_work(void* arg)
{
	int* client = (int*) arg;

	char filename[FILESIZE];
	recvReq(*client, filename);

	if(!isPresent(filename)) {

		int server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if(server_socket < 0) {
			printf("Server Socket is not created!\n");
			return NULL;
		}
		printf("=> Server Socket created\n");

		struct sockaddr_in server;
		server.sin_family         = AF_INET;
		server.sin_port           = htons(server_port);
		server.sin_addr.s_addr    = INADDR_ANY;

		if(connect(server_socket, (struct sockaddr*) &server, sizeof server) < 0) {
			printf("Connection error\n");
			return NULL;
		}

		char request[BUFFSIZE] = "GET /";
		strcat(request, filename);
		strcat(request, " HTTP/1.1\r\n\r\n");
		send(server_socket, request, sizeof request, 0);

		char response_server[BUFFSIZE];
		recv(server_socket, response_server, sizeof response_server, 0);
		create_update(response_server, filename);

		close(server_socket);

	}

	char response[BUFFSIZE];
	int status = check_file(filename, response);
	sendRes(*client, filename, response, status);
}

int main(int total, char* args[])
{
	if(total != 4) {
		printf("Use like this %s <server ip> <server port> <proxy port>\n", args[0]);
		return 1;
	}

	server_port = atoi(args[2]);
	if(server_port < 1000 || server_port > 2000) {
		printf("Please use Port number from 1000 to 2000 for Server\n");
		return 1;
	}

	unsigned short proxy_server_port = atoi(args[3]);
	if(proxy_server_port < 2000 || proxy_server_port > 3000) {
		printf("Please use Port number from 1000 to 2000 for Proxy Server\n");
		return 1;
	}

	printf("=> Ports accepted\n");

	int proxy_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(proxy_server_socket < 0) {
		printf("Socket is not created!\n");
		return 1;
	}
	printf("=> Socket created\n");

	struct sockaddr_in proxy_server;
	proxy_server.sin_family		= AF_INET;
	proxy_server.sin_port 		= htons(proxy_server_port); 
	proxy_server.sin_addr.s_addr	= INADDR_ANY;

	if(bind(proxy_server_socket, (struct sockaddr*) &proxy_server, sizeof proxy_server) < 0) {
		printf("Binding error\n");
		return 1;
	}
	printf("=> Proxy Server Binded to Port Number %u Successfull!\n", proxy_server_port);

	listen(proxy_server_socket, LISTENERS);
	printf("=> Listening to %d Listeners\n", LISTENERS);

	while(1) {
		int client = accept(proxy_server_socket, NULL, NULL);
		if(client != -1) {
			printf("Client connected!\n\n");
			pthread_t tid;
			pthread_create(&tid, NULL, client_work, &client);
			pthread_join(tid, NULL);
			close(client);
			printf("Client closed\n\n");
		}
	}
	close(proxy_server_socket);	
	return 0;
}
