#include "headers.h"

int cd(char **token)
{
    // char hol[200];
    char currdir[200];
    int ans;
    getcwd(currdir, sizeof(currdir));

    if (strcmp(token[1], "~") == 0 || token[1] == NULL || strcmp(token[1], "~/") == 0)
    {
        strcpy(prev_dir, currdir);
        ans = chdir(home_dir);
    }
    // add for home directory relative path
    else if (strcmp(token[1], "-") == 0)
    {
        printf("%s\n", prev_dir);
        ans = chdir(prev_dir);
        strcpy(prev_dir, currdir);
    }
    else if ((token[1]) != NULL && strcmp(token[1], "-") != 0)
    {
        strcpy(prev_dir, currdir);
        ans = chdir(token[1]);
    }
    if (ans < 0)
    {
        perror("cd");
        return -1;
    }
    else
        return 0;
}