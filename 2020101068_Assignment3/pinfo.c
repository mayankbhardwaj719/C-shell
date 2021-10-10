#include "headers.h"

void pinfo(int number_of_char, char *token[], char *home_directory)
{
	char *P_ID_S = malloc(20);
	if (number_of_char == 1)
	{
		pid_t pid = getpid();
		sprintf(P_ID_S, "%d", pid);
	}
	else if (number_of_char == 2)
	{
		strcpy(P_ID_S, token[1]);
	}
	else
	{
		fprintf(stderr, "Invalid number of arguments\n [usage] pinfo <pid>\n");
		return;
	}

	char *prev_directory = malloc(200);
	strcpy(prev_directory, "/proc/");
	strcat(prev_directory, P_ID_S);
	strcat(prev_directory, "/status");

	printf("pid -- %s\n", P_ID_S);

	FILE *procstat = fopen(prev_directory, "r");
	if (procstat)
	{
		char *buff = malloc(200);
		while (fgets(buff, 200, procstat))
		{
			if (!strncmp("State", buff, 5))
			{
				printf("Process status -- %s", buff + 7);
			}
			if (!strncmp("VmSize", buff, 6))
			{
				printf("memory -- %s", buff + 8);
			}
		}

		char *prev_directoryE = malloc(200);
		strcpy(prev_directoryE, "/proc/");
		strcat(prev_directoryE, P_ID_S);
		strcat(prev_directoryE, "/exe");

		char buff2[200] = "";

		if (readlink(prev_directoryE, buff2, 200) > 0)
		{
			addhome_directory(buff2, home_directory);
			printf("executable prev_directory -- %s\n", buff2);
		}
		else
		{
			perror("executable prev_directory");
		}
		free(prev_directoryE);
		free(buff);
	}
	else
	{
		perror("pinfo");
	}

	free(prev_directory);
	fclose(procstat);
}
