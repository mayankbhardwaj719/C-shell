#include "headers.h"

char *gethome_directory()
{
	char *home_directory = (char *) malloc(1024);
	int pid = getpid();
	char prev_directory[1024];
	sprintf(prev_directory, "/proc/%d/exe", pid);
	readlink(prev_directory, home_directory, 1024);
	int i=strlen(home_directory);
	while(1)
	{
		if(home_directory[i] != '/'){
			home_directory[i] = '\0';
		}
		else{
			home_directory[i] = '\0';
			break;
		}
		i--;
	}
	return home_directory;
}

void addhome_directory(char *curr_directory, char *home_directory)
{
	if(strlen(curr_directory) < strlen(home_directory)){
		return;
	}
	int i=0;
	while(i<strlen(home_directory))
	{
		if(curr_directory[i] != home_directory[i])
		{
			return;
		}
		i++;
	}
	curr_directory[0] = '~';
	for(int j = i; j < strlen(curr_directory); j++)
	{
		curr_directory[j-i+1] = curr_directory[j];
	}
	curr_directory[strlen(curr_directory) - i + 1] = '\0';
}

void display_directory(char *home_directory)
{
	struct passwd *pass = getpwuid(geteuid());// to provide information about a user account
	char *Host = malloc(50);
	gethostname(Host,50);	
	char *curr_directory = malloc(1024);
	getcwd(curr_directory,1024);
	addhome_directory(curr_directory, home_directory);
	printf("<");
	printf("%s@%s:", pass->pw_name, Host);
	printf("%s",curr_directory);
	printf("> ");
	free(Host);
	free(curr_directory);
}

