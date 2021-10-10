#include "headers.h"

#define BUF_SIZE 1024

void siginthandler(int signum)
{
	return;
}

void sigtstphandler(int signum)
{
	return;
}

int main()
{

	char *home_directory, **commands, *inputString;
	inputString = (char *) malloc(BUF_SIZE);
	commands = (char **) malloc(10*sizeof(char *));

	home_directory = gethome_directory();
	
	while (1) {
		disp_dir(home_directory);
		
		signal(SIGINT, siginthandler);
		signal(SIGTSTP, sigtstphandler);

		int size = getInput(inputString, commands);
		for (int i=0; i < size; i++) {
			int pipes[100][2];
			checkPiping(commands[i], home_directory, pipes, 0);
		}

		checkFinish();
	}
	return 0;
}
