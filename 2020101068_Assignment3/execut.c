#include "headers.h"

typedef struct processes
{
	int job_Num;
	int job_pid;
	char process_Name[1024];
	int curr_status; //1 for Running and 0 for stopped
	int indicator;	 //0 for foreground process and 1 for background process

} proc;
proc PCS[100]; //currently only check upto 100 processes

int process_size = 0;
int fg_process;
int ctrlZ = 0;

// setting up rules for comparison
// return strcmp(*((const char**)a)->process_Name, *((const char**)b)->process_Name);
int comparator(const void *p, const void *q)
{
	return strcmp(((proc *)p)->process_Name, ((proc *)q)->process_Name);
}

// // Function to sort the array
// void sort(const char *arr[], int n)
// {
// 	// calling qsort function to sort the array
// 	// with the help of Comparator
// qsort(arr, n, sizeof(const char *),comparator);
// }

void signalHandler2(int signum)
{
	ctrlZ = 1;
	kill(fg_process, SIGTSTP);
	return;
}

void checkFinish()
{
	int status;
	pid_t pid;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		int i = 1;
		// for (int i = 1; i <= process_size; i++)
		while (i <= process_size)
		{
			if (PCS[i].job_pid == pid)
			{
				PCS[i].curr_status = 0;
				if (WIFEXITED(status))
				{
					fprintf(stderr, "[%s with pid %d exited normally]\n", PCS[i].process_Name, pid);
				}
				else
				{
					fprintf(stderr, "[%s with pid %d exited abnormally]\n", PCS[i].process_Name, pid);
				}
			}
			i++;
		}
	}
	int j = 1, i = 1;
	// for (int i = 1; i <= process_size; i++)
	while (i <= process_size)
	{
		if (PCS[i].curr_status)
		{
			PCS[j++] = PCS[i];
			PCS[j - 1].job_Num = j - 1;
			PCS[j - 1].curr_status = 1;
		}
		i++;
	}
	process_size = j - 1;
	qsort(PCS, process_size, sizeof(struct processes *), comparator);
}

void Jobs(int number_of_char, char *token[])
{
	// for (int i = 1; i <= process_size; i++)
	qsort(PCS, process_size, sizeof(struct processes *), comparator);

	int flag_r = 0;
	int flag_s = 0;
	int flag_whole = 0;
	if (number_of_char == 1)
		flag_whole = 1;
	else if (number_of_char == 2)
	{
		if (strcmp(token[1], "-r") == 0)
			flag_r = 1;
		if (strcmp(token[1], "-s") == 0)
			flag_s = 1;
	}
	else
	{
		perror("invalid number of arguments\n");
		return;
	}
	int i = 1;
	while (i <= process_size)
	{
		char *prev_directory = malloc(100);
		sprintf(prev_directory, "/proc/%d/status", PCS[i].job_pid);

		char *PCStatus = malloc(1024);
		FILE *PCStat = fopen(prev_directory, "r");
		if (PCStat)
		{
			char *BUFFER = malloc(100);
			while (fgets(BUFFER, 100, PCStat))
			{
				if (!strncmp("State", BUFFER, 5))
				{
					break;
				}
			}
			int check = 0, j = 0;
			for (int i = 0; i < strlen(BUFFER); i++)
			{
				if (BUFFER[i] == ')')
				{
					check = 0;
					break;
				}
				if (check)
				{
					PCStatus[j++] = BUFFER[i];
				}
				if (BUFFER[i] == '(')
				{
					check = 1;
				}
			}
			PCStatus[j] = '\0';
		}
		else
		{
			PCS[i].curr_status = 0;
		}
		if (PCS[i].curr_status)
		{
			if (flag_whole)
				printf("[%d] %s %s [%d]\n", PCS[i].job_Num, strcmp(PCStatus, "stopped") ? "Running" : "Stopped", PCS[i].process_Name, PCS[i].job_pid);
			else
			{
				if (flag_r && strcmp(PCStatus, "stopped")!=0)
				{
					printf("[%d] %s %s [%d]\n", PCS[i].job_Num, strcmp(PCStatus, "stopped") ? "Running" : "Stopped", PCS[i].process_Name, PCS[i].job_pid);
				}
				else if(flag_s && strcmp(PCStatus, "stopped")==0)
				{
					printf("[%d] %s %s [%d]\n", PCS[i].job_Num, strcmp(PCStatus, "stopped") ? "Running" : "Stopped", PCS[i].process_Name, PCS[i].job_pid);
				}
			}
		}

		qsort(PCS, number_of_char, sizeof(struct processes), comparator);
		i++;
	}
}

void sig(int number_of_char, char *token[])
{
	if (number_of_char != 3)
	{
		perror("invalid number of arguments");
		return;
	}
	int job_ID = atoi(token[1]);
	if (job_ID > process_size || job_ID < 1)
	{
		perror("invalid job number");
		return;
	}
	int killSignal = atoi(token[2]);
	if (kill(PCS[job_ID].job_pid, killSignal) == -1)
	{
		perror("sig");
		return;
	}
}

void exec_fg(int number_of_char, char *token[])
{
	int status;
	if (number_of_char != 2)
	{
		perror("invalid number of arguments");
		return;
	}
	int job_ID = atoi(token[1]);
	if (job_ID > process_size || job_ID < 1)
	{
		perror("invalid job number");
		return;
	}
	if (getpgid(PCS[job_ID].job_pid) != getpid())
		tcsetpgrp(0, PCS[job_ID].job_pid);

	if (kill(PCS[job_ID].job_pid, SIGCONT) < 0)
	{
		perror("no such process\n");
		return;
	}
	signal(SIGTTOU, SIG_IGN);
	waitpid(-1, &status, WUNTRACED);

	if (tcgetpgrp(STDIN_FILENO) == -1)
	{
		perror("tcgetpgrp for stdin\n");
	}
	else if (tcsetpgrp(STDIN_FILENO, getpid()) == -1)
	{
		perror("tcsetpgrp for stdin\n");
	}
	signal(SIGTTOU, SIG_DFL);
	return;
}

void exec_bg(int number_of_char, char *token[])
{
	if (number_of_char != 2)
	{
		perror("invalid number of arguments");
		return;
	}
	int job_ID = atoi(token[1]);
	if (job_ID > process_size || job_ID < 1)
	{
		perror("invalid job number");
		return;
	}
	if (kill(PCS[job_ID].job_pid, SIGCONT) == -1)
	{
		perror("no such process");
	}
}

void execute(int number_of_char, char *token[], int bg)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork failed");
		return;
	}

	else if (pid == 0)
	{
		if (bg)
		{
			setpgid(0, 0);
		}
		execvp(token[0], token);
		perror("invalid command");
		_exit(1);
	}

	if (bg == 0)
	{
		int child_status = 0;
		fg_process = pid;
		signal(SIGTSTP, signalHandler2);
		waitpid(pid, &child_status, WUNTRACED);
		if (ctrlZ)
		{
			process_size++;
			PCS[process_size].job_Num = process_size;
			PCS[process_size].job_pid = pid;
			strcpy(PCS[process_size].process_Name, token[0]);
			for (int i = 1; i < number_of_char; i++)
			{
				strcat(PCS[process_size].process_Name, " ");
				strcat(PCS[process_size].process_Name, token[i]);
			}
			PCS[process_size].curr_status = 1;
			PCS[process_size].indicator = 1;
			ctrlZ = 0;
		}
	}
	else
	{
		process_size++;
		PCS[process_size].job_Num = process_size;
		PCS[process_size].job_pid = pid;
		strcpy(PCS[process_size].process_Name, token[0]);
		for (int i = 1; i < number_of_char; i++)
		{
			strcat(PCS[process_size].process_Name, " ");
			strcat(PCS[process_size].process_Name, token[i]);
		}
		PCS[process_size].curr_status = 1;
		PCS[process_size].indicator = 1;
		printf("pid=%d\n", pid);
	}

	return;
}

