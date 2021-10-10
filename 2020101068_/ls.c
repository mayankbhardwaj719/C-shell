#include "headers.h"

/*
**	ls executes the ls command
**	ls_normal is used for simple ls
**  ls_al is used for ls -al
**  ls_a is used for ls -a
**  ls_l is used for ls -l
*/

void ls(int number_of_char, char *token[], char *home_directory)
{
	char *directory = malloc(1024);
	strcpy(directory, ".");
	int flag_FOR_l = 0;
	int flag_FOR_a = 0;
	int possible = 0;
	for (int i = 1; i < number_of_char; i++)
	{
		if (token[i][0] == '-')
		{
			for (int j = 1; j < strlen(token[i]); j++)
			{
				if (token[i][j] == 'a')
				{
					flag_FOR_a = 1;
				}
				else if (token[i][j] == 'l')
				{
					flag_FOR_l = 1;
				}
				else
				{
					printf("Only ls -[al] implemented\n");
				}
			}
		}
		else
		{
			possible++;
			if (possible > 1)
			{
				printf("Too many token / Presently only possible for one directory\n");
				return;
			}
			strcpy(directory, token[i]);
		}
	}
	char *dummy_home_directory = malloc(1024);
	strcpy(dummy_home_directory, home_directory);
	if (directory[0] == '~')
	{
		strcat(dummy_home_directory, directory + 1);
		strcpy(directory, dummy_home_directory);
	}
	if (flag_FOR_a && flag_FOR_l)
	{
		ls_al(directory);
	}
	else if (flag_FOR_a)
	{
		ls_a(directory);
	}
	else if (flag_FOR_l)
	{
		ls_l(directory);
	}
	else
	{
		ls_normal(directory);
	}

	free(directory);
	free(dummy_home_directory);
}

void ls_normal(char *directory)
{
	struct dirent **All_Names;
	int n = scandir(directory, &All_Names, NULL, alphasort);
	if (n != -1)
	{
		while (n--)
		{
			if (All_Names[n]->d_name[0] != '.')
				printf("%s\n", All_Names[n]->d_name);
			free(All_Names[n]);
		}
	}
	else
	{
		perror("ls");
		return;
	}
	free(All_Names);
}

void ls_al(char *directory)
{
	struct dirent **All_Names;
	struct stat st;
	if (stat(directory, &st) != 0)
	{
		perror("ls");
		return;
	}
	printf("total %ld\n", st.st_size);
	int n = scandir(directory, &All_Names, NULL, alphasort);
	if (n == -1)
	{
		perror("ls");
		return;
	}
	int size_OF_directory = strlen(directory);
	while (n--)
	{
		directory[size_OF_directory] = '/';
		directory[size_OF_directory + 1] = '\0';
		strcat(directory, All_Names[n]->d_name);
		if (stat(directory, &st) != 0)
		{
			perror("ls");
			return;
		}
		printf((S_ISDIR(st.st_mode)) ? "d" : "-");
		printf((st.st_mode & S_IRUSR) ? "r" : "-");
		printf((st.st_mode & S_IWUSR) ? "w" : "-");
		printf((st.st_mode & S_IXUSR) ? "x" : "-");
		printf((st.st_mode & S_IRGRP) ? "r" : "-");
		printf((st.st_mode & S_IWGRP) ? "w" : "-");
		printf((st.st_mode & S_IXGRP) ? "x" : "-");
		printf((st.st_mode & S_IROTH) ? "r" : "-");
		printf((st.st_mode & S_IWOTH) ? "w" : "-");
		printf((st.st_mode & S_IXOTH) ? "x" : "-");
		printf(" ");
		printf("%3ld ", st.st_nlink);
		struct passwd *password = getpwuid(st.st_uid);
		printf("%12s ", password->pw_name);
		struct group *grp = getgrgid(st.st_gid);
		printf("%12s ", grp->gr_name);
		printf("%8ld ", st.st_size);
		char *date_string = malloc(50);
		strftime(date_string, 50, "%b %2d %R", localtime(&(st.st_mtime)));
		printf("%s ", date_string);
		printf("%s\n", All_Names[n]->d_name);
		free(date_string);
		free(All_Names[n]);
	}
	free(All_Names);
}

void ls_l(char *directory)
{
	struct dirent **All_Names;
	struct stat st;
	if (stat(directory, &st) != 0)
	{
		perror("ls");
		return;
	}
	printf("total %ld\n", st.st_size);
	int n;
	n = scandir(directory, &All_Names, NULL, alphasort);
	if (n == -1)
	{
		perror("ls");
		return;
	}
	int size_OF_directory = strlen(directory);
	while (n--)
	{
		if (All_Names[n]->d_name[0] == '.')
		{
			free(All_Names[n]);
			continue;
		}
		directory[size_OF_directory] = '/';
		directory[size_OF_directory + 1] = '\0';
		strcat(directory, All_Names[n]->d_name);
		if (stat(directory, &st) != 0)
		{
			printf("%s\n", directory);
			perror("ls");
			return;
		}
		printf((S_ISDIR(st.st_mode)) ? "d" : "-");
		printf((st.st_mode & S_IRUSR) ? "r" : "-");
		printf((st.st_mode & S_IWUSR) ? "w" : "-");
		printf((st.st_mode & S_IXUSR) ? "x" : "-");
		printf((st.st_mode & S_IRGRP) ? "r" : "-");
		printf((st.st_mode & S_IWGRP) ? "w" : "-");
		printf((st.st_mode & S_IXGRP) ? "x" : "-");
		printf((st.st_mode & S_IROTH) ? "r" : "-");
		printf((st.st_mode & S_IWOTH) ? "w" : "-");
		printf((st.st_mode & S_IXOTH) ? "x" : "-");
		printf(" ");
		printf("%3ld ", st.st_nlink);
		struct passwd *password = getpwuid(st.st_uid);
		printf("%12s ", password->pw_name);
		struct group *grp = getgrgid(st.st_gid);
		printf("%12s ", grp->gr_name);
		printf("%8ld ", st.st_size);
		char *date_string = malloc(50);
		strftime(date_string, 50, "%b %d %R", localtime(&(st.st_mtime)));
		printf("%s ", date_string);
		printf("%s\n", All_Names[n]->d_name);
		free(date_string);
		free(All_Names[n]);
	}
	free(All_Names);
}

void ls_a(char *directory)
{
	struct dirent **All_Names;
	int n = scandir(directory, &All_Names, NULL, alphasort);
	if (n != -1)
	{
		while (n--)
		{
			printf("%s\n", All_Names[n]->d_name);
			free(All_Names[n]);
		}
	}
	else
	{
		perror("ls");
		return;
	}

	free(All_Names);
}
