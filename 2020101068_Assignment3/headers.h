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

void run(char *inputString, char *home_directory);
void checkPiping(char *inputString, char *home_directory, int pipes[][2], int pipenum); 

void execLS(int number_of_char, char *token[], char *home_directory);
void sls(char *dir);
void slsa(char *dir);
void slsl(char *dir);
void slsal(char *dir);
void decideColor(int flag1,int flag2);
void resetColor();

int gettoken(char inputString[], char **token);
int getInput(char *inputString, char **commands);

char *gethome_directory();
void addhome_directory(char *currDir, char *home_directory);
void disp_dir(char *home_directory);	

// Builtin commands
int execBuiltin(int number_of_char, char *token[], char *home_directory);
void pwd();
void echo(char **token, int number_of_char);
void cd(char **token, int number_of_char, char *home_directory);

void execJobs(int number_of_char, char *token[]);
void exec_kjob(int number_of_char, char *token[]);
void exec_overkill(int number_of_char, char *token[]);
void exec_fg(int number_of_char, char *token[]);
void exec_bg(int number_of_char, char *token[]);
void execute(int number_of_char, char *token[], int bg);
void checkFinish();

void execPinfo(int number_of_char, char *token[], char *home_directory);

void insertHistory(char *inputString, char *home_directory);
void execHistory(int number_of_char, char *token[], char *home_directory);

void execNight(int number_of_char, char *token[]);
void exec_dirty(int sec);
void exec_interrupt(int sec);

int checkRedirect(int *number_of_char, char *token[], char *home_directory);


