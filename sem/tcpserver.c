#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int args, char* argv[])
{
	if(args != 2) {
		puts("ERR 0");
		return -1;
	}
	int serverd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverd == -1) {
		puts("ERR 1");
		return -1;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	unsigned short port = atoi(argv[1]);
	server.sin_port = htons(port);

	int b = bind(serverd, (struct sockaddr*)&server, sizeof(server));

	if(b == -1) {
		puts("ERR 2");
		return -1;
	}
	
	listen(serverd, 1);
	int client = accept(serverd, 0, 0);

	char msg[100];

	while(1) {
		memset(msg, 0, sizeof msg);
		recv(client, msg, sizeof(msg), 0);
		puts(msg);

		scanf("%[^\n]", msg);
		getchar();
		send(client, msg, sizeof(msg), 0);
	}
	return 0;
}
