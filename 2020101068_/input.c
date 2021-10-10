#include "headers.h"

#define DELIM_TOKEN " \t\r\f\a\n"
#define BUFFER_SIZE 1024  //assuming max length of input is BUFFER_SIZE

int getInput(char *inputString, char **commands)
{
	int size = 0;
	if(fgets(inputString, BUFFER_SIZE, stdin)==NULL)
	{	
		printf("\n");
		exit(0);
	}
	
	inputString[strlen(inputString)-1] = '\0';	//removing extra '\n'
	
	char *found_token = strtok(inputString, ";");
	while( found_token != NULL )
	{
		commands[size] = (char *)malloc(BUFFER_SIZE);
		strcpy(commands[size++], found_token);
		found_token = strtok(NULL, ";");
	}
	return size;
}

int gettoken(char *inputString, char **token)
{
	int size = 0;
	char *found_token;
	found_token = strtok(inputString, DELIM_TOKEN);
	while( found_token != NULL )
	{
		token[size] = (char *)malloc(BUFFER_SIZE);
		strcpy(token[size++], found_token);
		found_token = strtok(NULL, DELIM_TOKEN);
    }
    token[size] = NULL;

    free(found_token);
	
	return size;
}

