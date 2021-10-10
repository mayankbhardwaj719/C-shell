#include "headers.h"

void checkPiping(char *inputString, char *home_directory, int pipes[][2], int pipenum) 
{
	int pipecount = 0;
	for(int i = 0; i < strlen(inputString); i++) {
		if(inputString[i] == '|') {
			pipecount++;
		}
	}

	if(pipecount == 0) {
		run(inputString, home_directory);
		return;
	}

	pipe(pipes[pipenum]);
	int i;
	for(i = 0; inputString[i] != '|'; i++);
	inputString[i] = '\0';
	
	int spid = fork();
	if(spid == 0) {
		close(pipes[pipenum][0]);
		dup2(pipes[pipenum][1], 1); //duplicate write end to stdout
		close(pipes[pipenum][1]);
		run(inputString, home_directory);
		exit(0);
	} 
	else if (spid < 0) {
		perror("fork failed\n");
	} 
	else {
		int dpid = fork();
		if(dpid == 0) {
			close(pipes[pipenum][1]); //close write end
			dup2(pipes[pipenum][0], 0); //duplicate read end to stdin
			close(pipes[pipenum][0]);
			checkPiping(inputString + i + 1, home_directory, pipes, pipenum + 1);
			exit(0);
		} 
		else if (dpid < 0) {
			perror("fork failed");
		} 
		else {
			close(pipes[pipenum][0]);
			close(pipes[pipenum][1]);
			wait(NULL);
			wait(NULL);
			return;
		}
	}
}

void run(char *inputString, char *home_directory)
{
	char **token = (char **)malloc(10*sizeof(char *)); //Assuming max 10 token
	int number_of_char = gettoken(inputString, token);

	int fstdin, fstdout;
	if((fstdin = dup(0)) < 0) {
		perror("dup failed");
		return;
	}
	if((fstdout = dup(1)) < 0) {
		perror("dup failed");
		return;
	}

	if(checkRedirect(&number_of_char, token, home_directory) != 0) {
		return;
	}
	
	int bg = 0;
	if(!strcmp(token[number_of_char-1], "&")) {
		token[number_of_char-1] = NULL;
		number_of_char--;
		bg = 1;
	}

	if(!strcmp(token[0],"exit") || !strcmp(token[0],"quit")) {
		_exit(0);
	}

	if(Builtin(number_of_char, token, home_directory) != 0) {
		if(!strcmp(token[0], "ls")) {
			ls(number_of_char, token, home_directory);
		}
		else if(!strcmp(token[0], "pinfo")) {
			pinfo(number_of_char, token, home_directory);
		}
		else if(!strcmp(token[0], "nightswatch")) {
			execNight(number_of_char, token);
		}
		else if(!strcmp(token[0], "jobs")) {
			Jobs(number_of_char, token);
		}
		else if(!strcmp(token[0], "sig")) {
			sig(number_of_char, token);
		}
		else if(!strcmp(token[0], "fg")) {
			exec_fg(number_of_char, token);
		}
		else if(!strcmp(token[0], "bg")) {
			exec_bg(number_of_char, token);
		}
		else {
			execute(number_of_char, token, bg);
		}
		
	}
	fflush(stdout);
	fflush(stdin);
	dup2(fstdin, 0);
	dup2(fstdout, 1);
	close(fstdin);
	close(fstdout);
}