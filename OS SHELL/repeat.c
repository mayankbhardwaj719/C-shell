#include "headers.h"

void repeat(char **token,int k)
{
    
    int length = atoi(token[1]);
    
    for(int i=2;i<k;i++)
    {
        strcpy(token[i-2],token[i]);
    }

    for (int i = 0; i < length; i++)
    {
        if (strcmp(token[0], "pwd") == 0)
        {
            pwd();
        }
        else if (strcmp(token[0], "echo") == 0)
        {
            echo(token,k-2); 
        }
        else if (strcmp(token[0], "cd") == 0)
        {
            cd(token);
        }
        else if (strcmp(token[0], "ls") == 0)
        {
            ls(token); 
        }
        
    }

    return;

}      
    
    
