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
	int client = socket(AF_INET, SOCK_DGRAM, 0);
	if(client == -1) {
		puts("ERR 1");
		return -1;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	unsigned short port = atoi(argv[1]);
	server.sin_port = htons(port);

	char msg[100];

	while(1) {
		scanf("%[^\n]", msg);
		getchar();
		int n = sizeof(server);
		sendto(client, msg, sizeof(msg), 0, (struct sockaddr*)&server, n);

		memset(msg, 0, sizeof msg);
		recvfrom(client, msg, sizeof(msg), 0, (struct sockaddr*)&server, &n);
		puts(msg);
	}
	return 0;
}
