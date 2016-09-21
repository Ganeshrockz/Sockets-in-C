#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define BUFF		64
unsigned short Port;

void Client_Data_Connection(char* To_Be_Sent, char* To_Be_Recv, int Send)
{
	int FTP_Data_Server_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if(FTP_Data_Server_Socket == -1) {
		puts("ERR: Data socket is not created");
		return;
	}

	struct sockaddr_in FTP_Data_Server;
	FTP_Data_Server.sin_addr.s_addr      = INADDR_ANY;
	FTP_Data_Server.sin_port             = htons(Port+1);
	FTP_Data_Server.sin_family           = AF_INET;

	if(connect(FTP_Data_Server_Socket, (struct sockaddr*)&FTP_Data_Server, sizeof FTP_Data_Server) == -1) {
		puts("ERR: Data Connection error");
		return;
	}

	if(Send == 1) {
		send(FTP_Data_Server_Socket, To_Be_Sent, strlen(To_Be_Sent), 0);
	} else {
		recv(FTP_Data_Server_Socket, To_Be_Recv, BUFF*25, 0);
	}
	close(FTP_Data_Server_Socket);
}

void Analyse_Command(char* Command)
{
	if(Command[0] == 'g' && Command[1] == 'e' && Command[2] == 't' && Command[3] == ' ') {
		char FileName[BUFF], i, j;
		memset(FileName, 0, BUFF);
		for(i=4, j=0; Command[i]; ++i, ++j)
			FileName[j] = Command[i];
		FILE *FP = fopen(FileName, "w");
		if(FP) {
			char FileData[BUFF*25];
			memset(FileData, 0, BUFF*25);
			Client_Data_Connection(NULL, FileData, 0);
			fprintf(FP, "%s", FileData);
			fclose(FP);
		}
	} else if(Command[0] == 'p' && Command[1] == 'u' && Command[2] == 't' && Command[3] == ' ') {
		char FileName[BUFF], i, j;
		memset(FileName, 0, BUFF);
		for(i=4, j=0; Command[i]; ++i, ++j)
			FileName[j] = Command[i];
		if(access(FileName, 0) == 0) {
			FILE *FP = fopen(FileName, "r");
			if(FP) {
				char FileData[BUFF*25];
				fscanf(FP, "%[^EOF]", FileData);
				Client_Data_Connection(FileData, NULL, 1);
				fclose(FP);
			}
		}
	} else {
		char FileData[BUFF*25];
		memset(FileData, 0, BUFF*25);
		Client_Data_Connection(NULL, FileData, 0);
		puts(FileData);		
	}
}


void* Client_Control_Connection(void* Arg)
{
	int Client_Socket = *(int *)Arg;
	char Command[BUFF];
	char Ready[BUFF];

	while(1) {
		memset(Command, 0, BUFF);
		printf("> ");
		scanf("%[^\n]", Command);
		getchar();
		if(Command[0] == '!') {
			int i;
			for(i=0; Command[i+1]; ++i)
				Command[i] = Command[i+1];
			Command[i] = 0;
			system(Command);
		} else {

			send(Client_Socket, Command, strlen(Command), 0);
			memset(Ready, 0, BUFF);
			recv(Client_Socket, Ready, BUFF, 0);
			Analyse_Command(Command);
		}
	}
}

int main(int TotalArg, char* Args[])
{
	if(TotalArg != 2) {
		printf("Did you mean?\n");
		printf("%s <port>\n", Args[0]);
		return -1;
	}

	Port = atoi(Args[1]);

	int FTP_Server_Socket = socket(AF_INET, SOCK_STREAM, 0);	
	if(FTP_Server_Socket == -1) {
		puts("ERR: Socket is not created");
		return -1;
	}

	struct sockaddr_in FTP_Server;
	FTP_Server.sin_addr.s_addr	= INADDR_ANY;
	FTP_Server.sin_port		= htons(Port);
	FTP_Server.sin_family		= AF_INET;

	if(connect(FTP_Server_Socket, (struct sockaddr*)&FTP_Server, sizeof FTP_Server) == -1) {
		puts("ERR: Connection error");
		return -1;
	}

	Client_Control_Connection(&FTP_Server_Socket);

	close(FTP_Server_Socket);
	return 0;
}
