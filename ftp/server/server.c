#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define BUFF		64
#define LISTENERS	10
unsigned short Port;

void Client_Data_Connection(char* To_Be_Sent, char* To_Be_Recv, int Send, int FTP_Data_Server_Socket, int Client)
{
	send(Client, "ACTIVE", strlen("ACTIVE"), 0);	

	int Data_Client = accept(FTP_Data_Server_Socket, NULL, NULL);

	if(Send == 1) {
		send(Data_Client, To_Be_Sent, strlen(To_Be_Sent), 0);
	} else {
		memset(To_Be_Recv, 0, sizeof To_Be_Recv);
		recv(Data_Client, To_Be_Recv, BUFF*25, 0);
	}
}

void Execute_Command(char* Command, int FTP_Data_Server_Socket, int Client)
{ 
	if(Command[0] == 'g' && Command[1] == 'e' && Command[2] == 't' && Command[3] == ' ') {
		char FileName[BUFF], i, j;
		memset(FileName, 0, BUFF);
		for(i=4, j=0; Command[i]; ++i, ++j)
			FileName[j] = Command[i];
		printf("[%s]\n", FileName);
		if(access(FileName, 0) == 0) {
			FILE *FP = fopen(FileName, "r");
			if(FP) {
				char FileData[BUFF*25];
				fscanf(FP, "%[^EOF]", FileData);
				Client_Data_Connection(FileData, NULL, 1, FTP_Data_Server_Socket, Client);
				fclose(FP);
			}
		}
	} else if(Command[0] == 'p' && Command[1] == 'u' && Command[2] == 't' && Command[3] == ' ') {
		char FileName[BUFF], i, j;
		memset(FileName, 0, BUFF);
		for(i=4, j=0; Command[i]; ++i, ++j)
			FileName[j] = Command[i];
		FILE *FP = fopen(FileName, "w");
		if(FP) {
			char FileData[BUFF*25];
			Client_Data_Connection(NULL, FileData, 0, FTP_Data_Server_Socket, Client);
			fprintf(FP, "%s", FileData);
			fclose(FP);
		}
	} else {
		char FullCommand[BUFF];
		strcpy(FullCommand, Command);
		strcat(FullCommand, " > temp.txt");
		system(FullCommand);
		printf("[%s]\n", Command);
		FILE *FP = fopen("temp.txt", "r");
		if(FP) {
			char FileData[BUFF*25];
			fscanf(FP, "%[^EOF]", FileData);
			FileData[strlen(FileData)-1] = 0;
			Client_Data_Connection(FileData, NULL, 1, FTP_Data_Server_Socket, Client);
			fclose(FP);
		}
		system("rm temp.txt");
	}
}

void* Client_Control_Connection(void* Arg)
{
	int Client_Socket = *(int *)Arg;

	int FTP_Data_Server_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if(FTP_Data_Server_Socket == -1) {
		puts("ERR: Data socket is not created");
		return;
	}

	struct sockaddr_in FTP_Data_Server;
	FTP_Data_Server.sin_addr.s_addr      = INADDR_ANY;
	FTP_Data_Server.sin_port             = htons(Port+1);
	FTP_Data_Server.sin_family           = AF_INET;

	if(bind(FTP_Data_Server_Socket, (struct sockaddr*)&FTP_Data_Server, sizeof FTP_Data_Server) == -1) {
		puts("ERR: Data Binding error");
		return;
	}

	if(listen(FTP_Data_Server_Socket, LISTENERS) == -1) {
		puts("ERR: Data Listening error");
		return;
	}

	puts("SUC: Data Server is active");


	char Command[BUFF];

	while(1) {
		memset(Command, 0, BUFF);
		recv(Client_Socket, Command, BUFF, 0);
		Execute_Command(Command, FTP_Data_Server_Socket, Client_Socket);
	}
	
	close(FTP_Data_Server_Socket);
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

	if(bind(FTP_Server_Socket, (struct sockaddr*)&FTP_Server, sizeof FTP_Server) == -1) {
		puts("ERR: Binding error");
		return -1;
	}

	if(listen(FTP_Server_Socket, LISTENERS) == -1) {
		puts("ERR: Listening error");
		return -1;
	}

	puts("SUC: Server is active");

	while(1) {
		int Client_Socket = accept(FTP_Server_Socket, NULL, NULL);
		if(Client_Socket != -1) {
			pthread_t Client_Thread;
			pthread_create(&Client_Thread, NULL, Client_Control_Connection, &Client_Socket);
			pthread_join(Client_Thread, NULL);
			close(Client_Socket);
		}
	}	

	close(FTP_Server_Socket);
	return 0;
}
