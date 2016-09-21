#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE 2048

void getFileName(char* res, char* filename)
{
	if(res[0]=='G' && res[1]=='E' && res[2]=='T' && res[3]==' ') {
		if(res[4]=='/') {
			if(res[5]==' ') {	
				strcpy(filename, "index.html");
			} else {
				int size = 0, i = 5;
				while(res[i] != ' ')
					filename[size++] = res[i++];
			}
		}
	}
	return;
}

void getFileData(char* filename, char* res)
{
	if(access(filename, 0) == 0) {
		strcpy(res, "HTTP/1.1 GET OK\r\n\r\n");
		FILE* FP = fopen(filename, "r");
		if(FP) {
			char data[SIZE];
			fscanf(FP, "%[^EOF]", data);
			strcat(res, data);
			fclose(FP);
		}
	}
	else {
		strcpy(res, "HTTP/1.1 GET OK\r\n\r\n<html><head><title>Error</title></head><body><h1>404 Error</h1></body></html>");
	}

}

void* function(void* arg)
{
	int client = *(int *)arg;
	char req[SIZE];
	char res[SIZE];
	char filename[SIZE];
	char filedata[SIZE];
	memset(req, 0, sizeof req);
	recv(client, req, SIZE, 0);
	getFileName(req, filename);
	getFileData(filename, res);
	send(client, res, strlen(res), 0);
}

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

	if(bind(server_socket, (struct sockaddr*)&server, sizeof server) != 0) {
		puts("Binding error");
		return -1;
	}

	if(listen(server_socket, 5) != 0) {
		puts("Listen erroe");
		return -1;
	}
	puts("Server is active");

	char req[SIZE];
	char res[SIZE];
	char filename[SIZE];
	char filedata[SIZE];

	while(1) {
		int client = accept(server_socket, NULL, NULL);

		//Concurrent begin
		pthread_t pid;
		pthread_create(&pid, NULL, function, &client);
		pthread_join(pid, NULL);
		//Concurrent end

		/*//Iterative begin
			function(&client);
		*///Iterative end

		close(client);
	}

	close(server_socket);
	return 0;
}
