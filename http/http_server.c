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
	server.sin_addr.s_addr = INADDR_ANY;

	bind(network_socket, (struct sockaddr*)&server, sizeof(server));
	listen(network_socket, 2);
	while(1){
		int client = accept(network_socket, NULL, NULL);
		FILE *fp = fopen("index.html", "r");
		char res[BUFF*5] = "HTTP/1.1 GET OK\r\n\n";
		char data[BUFF*4] = "\0";
		fscanf(fp, "%[^EOF]", data);
		strcat(res, data);
		send(client, res, BUFF*5, 0);
		close(client);
	}
	return 0;
}
