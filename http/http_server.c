#include <stdio.h>
#include <stdlib.h>
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
        char connection[MAX];
        char user_agent[MAX];
        char accept[MAX];
        char accept_lang[MAX];
};

struct response
{
	char version[MAX];
	char status_msg[MAX];
	char date[MAX];
	char server[MAX];
	char last_mod[MAX];
	char content_type[MAX];
	char data[MAX];
};

int main(void)
{
	int network_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(3000);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(network_socket, (struct sockaddr*)&server_address, sizeof(server_address));	

	listen(network_socket, 2);

	while(1)
	{
		int client_socket = accept(network_socket, NULL, NULL);
		recv(client_socket, &sample, sizeof(sample), 0);
		printf("a is %d\n", sample.a);
		printf("buff is %s\n", sample.buff);
		close(client_socket);
	}
	return 0;
}
