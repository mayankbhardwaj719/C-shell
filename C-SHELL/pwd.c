#include "pwd.h"
#include "headers.h"

void pwd()
{   
    int error;
    char currentdir[FILENAME_MAX];
    getcwd(currentdir, sizeof(currentdir));
    if (getcwd(currentdir, sizeof(currentdir)))
    {
        printf("%s\n", currentdir);
    }
    else
    {
        error = 1;
        perror("Error getting current directory");
    }
    return;
}
