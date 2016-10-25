#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define HSIZE	50
#define ISIZE	20

int main(int args, char* argv[])
{
	if(args != 2) {
		puts("ERR 0");
		return -1;
	}
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in LNS;
	LNS.sin_family		= AF_INET;
	LNS.sin_addr.s_addr	= INADDR_ANY;
	LNS.sin_port		= htons(atoi(argv[1]));

	char host[HSIZE];
	char IP[ISIZE];
	int ssize = sizeof(LNS);

	scanf("%s", host);
	sendto(sock, host, sizeof(host), 0, (struct sockaddr*)&LNS, ssize);

	memset(IP, 0, sizeof IP);
	recvfrom(sock, IP, sizeof(IP), 0, (struct sockaddr*)&LNS, &ssize);

	if(!strcmp(IP, "0"))
		printf("Not Found!");
	else
		puts(IP);

	return 0;
}
