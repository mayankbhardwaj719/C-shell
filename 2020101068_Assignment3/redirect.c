#include "headers.h"

//ONlY ONE OUTPUT AND ONE INPUT FILE SUPPORTED
int checkRedirect(int *number_of_char, char *token[], char *home_directory) 
{
	int in = 0, out = 0, outtype = 0, newnumber_of_char = -1;
	char *infile = malloc(1024);
	char *outfile = malloc(1024);

	for(int i = 0; i < *number_of_char; i++) {
		if(!strcmp(token[i], ">") || !strcmp(token[i], ">>")) {
			out++;
			if(!strcmp(token[i], ">>")) {
				outtype = 1;
			}
			if(out > 1) {
				perror("only one output redirection allowed");
				return 1;
			}
			if(i == *number_of_char - 1) {
				perror("no file given");
				return 1;
			}
			strcpy(outfile, token[i+1]);
			if(newnumber_of_char == -1) {
				newnumber_of_char = i;
			}
		}
		if(!strcmp(token[i], "<")) {
			in++;
			if(in > 1) {
				perror("only one input redirection allowed");
				return 1;
			}
			if(i == *number_of_char - 1) {
				perror("no file given");
				return 1;
			}
			strcpy(infile, token[i+1]);
			if(newnumber_of_char == -1) {
				newnumber_of_char = i;
			}
		}
	}
	if(in) {
		if(infile[0] == '~') {
			char *temp = malloc(1024);
			strcpy(temp, infile);
			strcpy(infile, home_directory);
			strcat(infile, temp+1);
			free(temp);
		}

		int fd1 = open(infile, O_RDONLY);
		if(fd1 < 0) {
			perror("Failed to open file / no such file exists");
			return 1;
		}
		close(0);
		if(dup2(fd1, 0) != 0) {
			perror("input dup2 failed");
			return 1;
		}
		close(fd1);
	}
	if(out) {
		if(outfile[0] == '~') {
			char *temp = malloc(1024);
			strcpy(temp, outfile);
			strcpy(outfile, home_directory);
			strcat(outfile, temp+1);
			free(temp);
		} 

		int fd2;
		if(outtype) fd2 = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else fd2 = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if(fd2 < 0) {
			perror("Failed to open file");
			return 1;
		} 
		close(1);
		if(dup2(fd2, 1) != 1) {
			perror("output dup2 failed");
			return 1;
		}
		close(fd2);
	}
	if(in || out) {
		*number_of_char = newnumber_of_char;
		token[*number_of_char] = NULL;
	}
	return 0;
}