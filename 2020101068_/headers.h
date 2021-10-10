#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

// Builtin commands ----> done
int Builtin(int number_of_char, char *token[], char *home_directory);
void pwd();
void echo(char **token, int number_of_char);
void cd(char **token, int number_of_char, char *home_directory);

// displaying commands ---> done
char *gethome_directory();
void addhome_directory(char *curr_directory, char *home_directory);
void display_directory(char *home_directory);	

// getting input and token ----> done
int gettoken(char inputString[], char **token);
int getInput(char *inputString, char **commands);

// ls command ----> done
void ls(int number_of_char, char *token[], char *home_directory);
void ls_normal(char *directory);
void ls_a(char *directory);
void ls_l(char *directory);
void ls_al(char *directory);

// pinfo command ---> done
void pinfo(int number_of_char, char *token[], char *home_directory);

// bg,fg, and jobs handling ----> done
void Jobs(int number_of_char, char *token[]);
void sig(int number_of_char, char *token[]);
void exec_fg(int number_of_char, char *token[]);
void exec_bg(int number_of_char, char *token[]);
void execute(int number_of_char, char *token[], int bg);
void checkFinish();

// baywatch ---> done
void baywatch(int number_of_char, char *token[]);
void exec_dirty(int sec);
void exec_interrupt(int sec);

void run(char *inputString, char *home_directory);
void checkPiping(char *inputString, char *home_directory, int pipes[][2], int pipenum); 



int checkRedirect(int *number_of_char, char *token[], char *home_directory);



