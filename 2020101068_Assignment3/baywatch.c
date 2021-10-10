#include "headers.h"

void baywatch(int number_of_char, char *token[])
{
	if(number_of_char == 2)
	{
		if(!strcmp(token[1], "dirty")){
			exec_dirty(1);
		}
		else if(!strcmp(token[1], "interrupt")) {
			exec_interrupt(5);
		}
		else {
			fprintf(stderr, "baywatch: invalid argument\n");
		}
	}
	else if(number_of_char == 4)
	{
		if(!strcmp(token[1], "-n"))
		{
			int sec = atoi(token[2]);
			if(!strcmp(token[3], "dirty")){
				exec_dirty(sec);
			}
			else if(!strcmp(token[3], "interrupt")) {
				exec_interrupt(sec);
			}
			else {
				fprintf(stderr, "baywatch: invalid argument\n");
			}
		}
		else{
			fprintf(stderr, "baywatch: invalid argument\n");
		}
	}
	else{
		fprintf(stderr, "baywatch: invalid number of arguments\n");
	}
}

void exec_dirty(int sec)
{
	int pid = fork();
	if(pid == 0) {
		while(1) {
			FILE *f = fopen("/proc/meminfo", "r");
			char buff[1024];
			if(f) {
				while(fgets(buff, 1024, f)) {
					if(!strncmp(buff, "Dirty", 5)) {
						fprintf(stderr, "%s", buff + 20);
					}
				}
				fclose(f);
			} else{
				perror("dirty file");
			}
			sleep(sec);
		}
	}
	else{
		char c;
		while(1){
			scanf("%c", &c);
			if(c == 'q'){
				kill(pid, SIGKILL);
				break;
			}
		}
	}
}

void exec_interrupt(int sec)
{
	int pid = fork();
	if(pid == 0) {
		FILE *f = fopen("/proc/interrupts", "r");
		char buff[1024];
		int size = 0;	
		if(f) {
			fgets(buff, 1024, f);
			char *found = strtok(buff, " ");
			while(found != NULL) {
				printf("%5s ", found);
				size++;
				found = strtok(NULL, " ");
			}
		}
		else{
			fprintf(stderr, "dirty file\n");
		}
		fclose(f);
		while(1) {
			
			FILE *f = fopen("/proc/interrupts", "r");
			char buff[1024];
			if(f) {
				fgets(buff, 1024, f);
				fgets(buff, 1024, f);
				fgets(buff, 1024, f);
				char *found2 = strtok(buff, " ");
				int j = 0;
				while(found2 != NULL) {
					if(j!=0)printf("%5s ", found2);
					j++;
					found2 = strtok(NULL, " ");
				}
				fclose(f);
			} else{
				perror("dirty file");
			}
			sleep(sec);
		}
	}
	else{
		char c;
		while(1){
			scanf("%c", &c);
			if(c == 'q'){
				kill(pid, SIGKILL);
				break;
			}
		}
	}
}