#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define BUFF 265

int main(void)
{
	int network_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(3000);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(network_socket, (struct sockaddr*)&server, sizeof(server));
	listen(network_socket, 2);
	int client = accept(network_socket, NULL, NULL);
	char filename[BUFF];
	recv(client, &filename, BUFF, 0);
	FILE *fp = fopen(filename, "r");
	char data[BUFF*4] = "\0";
	if(fp == NULL)
		printf("File not found\n");
	else
		fscanf(fp, "%[^EOF]", data);
	send(client, data, BUFF*4, 0);
	return 0;
}
