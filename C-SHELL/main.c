#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "headers.h"

#define HOST_NAME_MAX 1024
#define LOGIN_NAME_MAX 1024
size_t combytes = 1000;
size_t MAX_BYTE = 2048;

int main()
{
    int result;
    result = gethostname(hostname, HOST_NAME_MAX);
    if (result)
    {
        perror("gethostname");
        return EXIT_FAILURE;
    }
    result = getlogin_r(username, LOGIN_NAME_MAX);
    if (result)
    {
        perror("getlogin_r");
        return EXIT_FAILURE;
    }
    if (result < 0)
    {
        perror("printf");
        return EXIT_FAILURE;
    }

    getcwd(cwd, sizeof(cwd));
    getcwd(home_dir, sizeof(home_dir));
    getcwd(prev_dir, sizeof(prev_dir));
    strcpy(home_dir, cwd);

    while (1)
    {
        prompt();
        int i = 0;
        //*************************** Taking input here and tokenising it ***************************//

        UserCommand = (char *)malloc(combytes);
        getline(&UserCommand, &combytes, stdin);
        tok = strtok(UserCommand, ";\n"); // Tokenize input
        if (tok != NULL)
        {
            while (tok != NULL)
            {
                strcpy(Final_Command[i], tok);
                i++;
                tok = strtok(NULL, ";\n");
            }
        }

        //*************************** Taking input here and tokenising it ***************************//

        //*************************** Executing different commands after tokenising ***************************//
        for (int j = 0; j < i; j++)
        {
            Individual_Command = (char **)malloc(MAX_BYTE);
            Individual_Command[0] = strtok(Final_Command[j], " \t\r\n");
            int k = 0;
            while (Individual_Command[k] != NULL) //Separating Individual_Commands within the command
            {
                k++;
                Individual_Command[k] = strtok(NULL, " \t\r\n");
            }
            if (strcmp(Individual_Command[0], "exit") == 0 || strcmp(Individual_Command[0], "quit") == 0)
            {
                exit(0);
            }
            else if (strcmp(Individual_Command[0], "pwd") == 0)
            {
                pwd();
            }
            else if (strcmp(Individual_Command[0], "echo") == 0)
            {
                echo(Individual_Command, k); // k is the number of tokens
            }
            else if (strcmp(Individual_Command[0], "cd") == 0)
            {
                cd(Individual_Command);
            }
            else if (strcmp(Individual_Command[0], "ls") == 0)
            {
                ls(Individual_Command); // k is the number of tokens
            }
            else if (strcmp(Individual_Command[0], "repeat") == 0)
            {
                repeat(Individual_Command, k); // k is the number of tokens
            }
            
            else if (strcmp(Individual_Command[0], "pinfo") == 0) //COMMAND :PINFO
            {
                pinfo(k, Individual_Command);
            }
            else
            {
                bg_fg(Individual_Command, 0, k);
            }
        }
    }
}
