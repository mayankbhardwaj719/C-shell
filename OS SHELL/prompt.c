#include "prompt.h"
#include "headers.h"

void prompt()
{   
    int error;
    if (getcwd(cwd, sizeof(cwd)))
    {
        if(strcmp(cwd,home_dir)==0)
        {
            char *path = cwd;
            path += strlen(home_dir);
            printf("<%s@%s: ~%s>", username, hostname, path);
        }
        else
            printf("<%s@%s: %s>", username, hostname, cwd);
    }
    else
    {
        error = 1;
        perror("Error getting current directory");
    }
}
