#include "headers.h"

#define BUF_SIZE 1024

void SIGINT_HANDLE(int SIG_NUMBER)
{
	return;
}
void SIGTSTP_HANDLE(int SIG_NUMBER)
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
		display_directory(home_directory);
		signal(SIGINT, SIGINT_HANDLE);
		signal(SIGTSTP, SIGTSTP_HANDLE);

		int size = getInput(inputString, commands);
		for (int i=0; i < size; i++) 
		{
			int pipes[100][2];
			checkPiping(commands[i], home_directory, pipes, 0);
		}

		checkFinish();
	}
	return 0;
}
