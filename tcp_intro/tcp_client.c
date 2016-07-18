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

	int status = connect(network_socket, (struct sockaddr*)&server_address, sizeof(server_address));	
	if(status==-1){
		printf("Error in Connection\n");
	}

	char data[256];
	recv(network_socket, &data, sizeof(data), 0);

	printf("Recieved: %s\n", data);

	close(network_socket);

	return 0;
}
