#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
	int network_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(3000);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(network_socket, (struct sockaddr*)&server_address, sizeof(server_address));	

	listen(network_socket, 2);

	int client_socket = accept(network_socket, NULL, NULL);

	char data[256] = "F**k it works :p";
	send(client_socket, data, sizeof(data), 0);

	close(network_socket);

	return 0;
}
