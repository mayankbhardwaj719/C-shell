#include "headers.h"

#define BUF_SIZE 1024

int execBuiltin(int number_of_char, char *token[], char *home_directory)
{
	if (!strcmp(token[0], "cd")){
		cd(token, number_of_char, home_directory);
		return 0;
	}
	else if(!strcmp(token[0], "echo")){
		echo(token, number_of_char);
		return 0;
	}
	else if(!strcmp(token[0], "pwd")){
		pwd();
		return 0;
	}
	return 1;
}

void pwd()
{   
    int error;
    char currentdir[FILENAME_MAX];
    getcwd(currentdir, sizeof(currentdir));
    if (getcwd(currentdir, sizeof(currentdir)))
    {
        printf("%s\n", currentdir);
    }
    else
    {
        error = 1;
        perror("Error getting current directory");
    }
    return;
}

void echo(char **token, int number_of_char)
{
	for (int i = 1; i < number_of_char - 1; i++)
    {
        printf("%s ", token[i]);
    }
    printf("%s", token[number_of_char - 1]);
    printf("\n");
    return;
}

void cd(char **token, int number_of_char, char *home_directory)
{
	if(number_of_char > 2){
		printf("cd: string not in the current working directory: %s\n", token[0]);
		return;
	}
	char *path = malloc(BUF_SIZE);
	
	if(number_of_char == 1){
		strcpy(path, home_directory);
		if(chdir(path) != 0){
			perror("cd");
		}
		return;
	}
	
	if(token[1][0] == '~'){
		int i,j;
		for(i=0; i < strlen(home_directory); i++){
			path[i] = home_directory[i];
		}
		for(j = 1;j < strlen(token[1]); j++){
			path[i++] = token[1][j];
		}
	}
	else {
		strcpy(path, token[1]);
	}

	if(chdir(path) != 0){
		perror("cd");
	}

	free(path);
}