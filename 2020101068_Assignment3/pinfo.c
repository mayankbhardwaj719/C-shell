#include "headers.h"

void execPinfo(int number_of_char, char *token[], char *home_directory)
{
	char *pids = malloc(20);
	if(number_of_char == 1) {
		pid_t pid = getpid();
		sprintf(pids, "%d", pid);
	}
	else if(number_of_char == 2){
		strcpy(pids, token[1]);
	}
	else{
		fprintf(stderr, "Invalid number of arguments\n [usage] pinfo <pid>\n");
		return;
	}
	
	char *path = malloc(100);
	strcpy(path, "/proc/");
	strcat(path, pids);
	strcat(path, "/status");
	
	printf("pid -- %s\n", pids);

	FILE *procstat = fopen(path, "r");
	if(procstat) {
		char *buff = malloc(100);
		while(fgets(buff, 100, procstat)) {
			// printf("%s\n", buff);
			if(!strncmp("State", buff, 5)) {
				printf("Process status -- %s", buff+7);
			}
			if(!strncmp("VmSize", buff, 6)) {
				printf("memory -- %s", buff + 8);
			}
		}
		
		char *pathE = malloc(100);
		strcpy(pathE, "/proc/");
		strcat(pathE, pids);
		strcat(pathE, "/exe");
		
		char buff2[100] = "";

		if(readlink(pathE, buff2, 100) > 0){
			addhome_directory(buff2, home_directory);
			printf("executable path -- %s\n", buff2);
		}
		else{
			perror("executable path");
		}
		free(pathE);
		free(buff);
	}
	else{
		perror("pinfo");
	}

	free(path);
	fclose(procstat);
}	
