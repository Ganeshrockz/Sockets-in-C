#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAX 20

struct request
{
	char method[MAX];
	char url[MAX];
	char version[MAX];
//	char connection[MAX];
//	char user_agent[MAX];
//	char accept[MAX];
//	char accept_lang[MAX];
};

struct response
{
//	char status_msg[MAX];
//	char server[MAX];
//	char date[MAX];
//	char content_type[MAX];
//	char content_length[MAX];
//	char last_mod[MAX];
	char data[MAX*100];
};

int main(void)
{
	int network_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family 	= AF_INET;
	server_address.sin_port 	= htons(8000);
	server_address.sin_addr.s_addr 	= INADDR_ANY;

	int status = connect(network_socket, (struct sockaddr*)&server_address, sizeof(server_address));	
	if(status==-1){
		printf("Error in Connection\n");
	}
	struct request Req;
	char req[MAX] = "\0";
	struct response Res;
	
	printf("Enter Method: ");
	scanf("%s", Req.method);
	strcat(req, Req.method);
	strcat(req, " ");

	printf("Enter URL: ");
        scanf("%s", Req.url);
	strcat(req, Req.url);
	strcat(req, " HTTP/");

	printf("Enter Version: ");
	scanf("%s", Req.version);
	strcat(req, Req.version);
	strcat(req, "\r\n\r\n");
	puts(req);
	send(network_socket, req, sizeof(req), 0);
	int l;
	do{
		l = recv(network_socket, &Res.data, sizeof(Res.data), 0);
		if(l) puts(Res.data);
	}while(l);
	close(network_socket);
	return 0;
}
