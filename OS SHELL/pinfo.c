#include "headers.h"

void pinfo(int k, char **token)
{
    int memory;
    int file_ptr, f2;
    char buf[1000000], file[10000], c, status, temp[100000];
    pid_t pid;

    if (k == 1)
        pid = getpid(); //GETTING PID
    else
        pid = atoi(token[1]);
    sprintf(file, "/proc/%d/stat", pid); //For status, memory

    file_ptr = open(file, O_RDONLY);
    f2 = open(file, O_RDONLY);
    if (file_ptr < 0)
        perror("Error: no such process exists\n");
    else
    {
        FILE *fd;
        fd = fopen(file, "r");
        fscanf(fd, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lld %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status, &memory);
        fclose(fd);
        printf("PID -- %d\n", pid);
        printf("Process Status -- %c\n", status);
        printf("Memory -- %lld\n", memory);

        sprintf(file, "/proc/%d/exe", pid); // For executable path
        int x = readlink(file, buf, sizeof(buf));
        buf[x] = '\0';

        x=0;
        int ii;
        char buf2[100000];
        if (strcmp(buf, home_dir) == 0)
            strcpy(buf, "~");
        else
        {

            for (ii = 0; home_dir[ii]; ii++)
            {
                if (home_dir[ii] != buf[ii])
                {
                    x = ii;
                    break;
                }
            }
            if (x == 0)
            {
                buf2[0] = '~';
                int p, j;
                for (p = 1, j = ii; buf[j]; p++, j++)
                {
                    buf2[p] = buf[j];
                }
                buf2[p] = '\0';
            }

            else
                strcpy(buf2, buf);
        }

        printf("Executable Path---  %s\n", buf2);
    }
}
