#include "headers.h"

char *gethome_directory()
{
	char *home_directory = (char *) malloc(1024);
	
	int pid = getpid();
	char path[1024];
	sprintf(path, "/proc/%d/exe", pid);
	readlink(path, home_directory, 1024);
	for(int i=strlen(home_directory);;i--){
		if(home_directory[i] != '/'){
			home_directory[i] = '\0';
		}
		else{
			home_directory[i] = '\0';
			break;
		}
	}
	return home_directory;
}

void addhome_directory(char *currDir, char *home_directory)
{
	if(strlen(currDir) < strlen(home_directory)){
		return;
	}
	int i = 0;
	for(i=0; i<strlen(home_directory); i++){
		if(currDir[i] != home_directory[i]){
			return;
		}
	}
	currDir[0] = '~';
	for(int j = i; j < strlen(currDir); j++){
		currDir[j-i+1] = currDir[j];
	}
	currDir[strlen(currDir) - i + 1] = '\0';
}

void disp_dir(char *home_directory)
{
	struct passwd *pass = getpwuid(geteuid());
	char *Host = malloc(50);
	gethostname(Host,50);
	
	char *currDir = malloc(1024);
	getcwd(currDir,1024);
	addhome_directory(currDir, home_directory);
	
	printf("\033[1;31m");
	printf("<");
	printf("\033[1;32m");
	printf("%s@%s:", pass->pw_name, Host);
	printf("\033[1;34m");
	printf("%s",currDir);
	printf("\033[1;31m");
	printf("> ");
	printf("\033[0m");
	
	// free(pass);
	free(Host);
	free(currDir);
}

