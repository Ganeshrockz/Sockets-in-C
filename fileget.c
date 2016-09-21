#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void timestamp(char* filename, char *response)
{
        char command[1024] = "ls -l | grep ";
        strcat(command, filename);
        strcat(command, " | awk '{print $6,$7,$8,$9}' > .timestamp");
        system(command);
	FILE *F = fopen(".timestamp", "r");
	fgets(response, 1024, F);
	response[strlen(response)-1] = 0;
	system("rm .timestamp");
}

int main(void)
{	
	//Get filename from the client
	//Get them in these variables
	char response[1024];
	char filename[1024];
	scanf("%s", filename);
	timestamp(filename, response);
	puts(response);
	return 0;
}
