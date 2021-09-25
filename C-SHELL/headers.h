#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <signal.h>
#include "pwd.h"
#include "prompt.h"
#include "echo.h"
#include "cd.h"
#include "ls.h"
#include "repeat.h"
#include "bg_fg.h"
#include "pinfo.h"

char hostname[1024];
char username[1024];
char home_dir[256];
char prev_dir[256];
char *UserCommand;
char *tok;
char Final_Command[64][64];
char **Individual_Command;
char cwd[1024];
char  his_path[200];



struct Node
{
    pid_t pid;
    char name[1024];
    int job;
    struct Node *next;
};
int shellInFile, shellOutFile;
struct Node *head;



#endif