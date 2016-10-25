#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define HSIZE	50
#define ISIZE	20

int main(int args, char* argv[])
{
	if(args != 3) {
		puts("ERR 0");
		return -1;
	}

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == -1) {
		puts("ERR 1");
		return -1;
	}

	struct sockaddr_in LNS;
	LNS.sin_family		= AF_INET;
	LNS.sin_addr.s_addr	= INADDR_ANY;
	LNS.sin_port		= htons(atoi(argv[1]));

	char host[HSIZE];
	char IP[ISIZE];
	strcpy(IP, "0");

	int ssize = sizeof(LNS);

	int b = bind(sock, (struct sockaddr*)&LNS, ssize);
	if(b == -1)  {
		puts("ERR 2");
		return -1;
	}

	memset(host, 0, sizeof host);
	recvfrom(sock, host, sizeof(host), 0, (struct sockaddr*)&LNS, &ssize);
	printf("Checking %s in RECLNS\n", host);

	FILE *FP = fopen("RECLNS", "r");
	char fhost[HSIZE];
	char fIP[HSIZE];

	while(fscanf(FP, "%s %s", fhost, fIP) != EOF) {
		if(!strcmp(fhost, host)) {
			strcpy(IP, fIP);
		}
	}
	fclose(FP);

	if(!strcmp(IP, "0")) {
		printf("Sending ANS\n");
		struct sockaddr_in ANS;
		ANS.sin_family          = AF_INET;
		ANS.sin_addr.s_addr     = INADDR_ANY;
		ANS.sin_port            = htons(atoi(argv[2]));
		sendto(sock, host, sizeof(host), 0, (struct sockaddr*)&ANS, ssize);

		memset(IP, 0, sizeof IP);
		recvfrom(sock, IP, sizeof(IP), 0, (struct sockaddr*)&ANS, &ssize);

		if(!strcmp(IP, "0")) {
			printf("Not Found!\n");
		}
		else {
			printf("Updating RECLNS\n");
			FP = fopen("RECLNS", "a");
			fprintf(FP, "%s %s\n", host, IP);
			fclose(FP);
			printf("Sending %s \n", IP);
		}
	}	
	else {
		printf("Found! %s \n", IP);
	}

	sendto(sock, IP, sizeof(IP), 0, (struct sockaddr*)&LNS, ssize);
	
	return 0;
}
