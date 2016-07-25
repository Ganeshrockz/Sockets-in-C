#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct MyStruct
{
	int a;
	char buff[21];
}sample;

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
	printf("Enter for a: ");
	scanf("%d", &sample.a);
	printf("Enter for buff: ");
	scanf("%s", sample.buff);
	send(network_socket, (struct MyStruct*)&sample, sizeof(sample), 0);
	close(network_socket);
	return 0;
}
