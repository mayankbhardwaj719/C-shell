#include "headers.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

struct stat filestat;
struct tm *filetime;

void get_print_info(char * filename,char *file)
{
    char type = ' ';
    stat(filename,&filestat);
    if (S_ISREG(filestat.st_mode))
        type='-';
    else if (S_ISDIR(filestat.st_mode))
        type='d';
    else if (S_ISSOCK(filestat.st_mode))
        type='s';
    else if (S_ISLNK(filestat.st_mode))
        type='l';
    else if (S_ISBLK(filestat.st_mode))
        type='b';
    else if (S_ISCHR(filestat.st_mode))
        type='c';

    printf("%c",type);

    if (filestat.st_mode & S_IRUSR)
        printf("r");
    else 
        printf("-");
    if (filestat.st_mode & S_IWUSR)
        printf("w");
    else 
        printf("-");
    if (filestat.st_mode & S_IXUSR)
        printf("x");
    else 
        printf("-");
    if (filestat.st_mode & S_IRGRP)
        printf("r");
    else 
        printf("-");
    if (filestat.st_mode & S_IWGRP)
        printf("w");
    else 
        printf("-");
    if (filestat.st_mode & S_IXGRP)
        printf("x");
    else 
        printf("-");
    if (filestat.st_mode & S_IROTH)
        printf("r");
    else 
        printf("-");
    if (filestat.st_mode & S_IWOTH)
        printf("w");
    else 
        printf("-");
    if (filestat.st_mode & S_IXOTH)
        printf("x");
    else 
        printf("-");
    printf(" ");

    printf("%3ld ",filestat.st_nlink);
    printf("%12s ", getpwuid(filestat.st_uid)->pw_name);
	printf("%12s ", getgrgid(filestat.st_gid)->gr_name);
    printf("%8ld ",filestat.st_size);
    filetime = localtime(&(filestat.st_mtime));
    char datestring[200];
    strftime(datestring,sizeof(datestring),"%b %2d %H:%M ",filetime);
    printf("%s ",datestring);
    printf("%s\n",file);
    
}


int filter (const struct dirent * list)
{
    if (list->d_name[0]=='.')
        return 0 ;
    else 
        return 1;
}

void lsl (int aflag,char ** cmdarg,int dirnamflag)
{
    struct dirent ** list ;
    int k=0;
    if (dirnamflag==0)
    {
        if (aflag)
            k=scandir(".",&list,NULL,alphasort);
        else 
            k=scandir(".",&list,filter,alphasort);

        if (k < 0 )
            perror("ls directory ");
        else
        {
            int count =0;
            int total=0;
            while (count < k)
            {
                stat(list[count]->d_name,&filestat);
                total+=filestat.st_blocks;
                count ++;
            }
            count =0;
            printf ("Total %d\n",total/2);
            while (count < k)
            {
                total+=filestat.st_blocks;
                get_print_info(list[count]->d_name,list[count]->d_name);
                count ++;
            }
        }
        free(list);
    }
    else 
    {
        for (int j=1;cmdarg[j]!=NULL ;j++)
        {
            if (cmdarg[j][0]!='-')
            {
            if (!aflag   && cmdarg[j][0]!='-')
                k=scandir(cmdarg[j],&list,filter,alphasort);
            else if (aflag && cmdarg[j][0]!='-' )
            {
                k=scandir(cmdarg[j],&list,NULL,alphasort);
            }
            if (k < 0 )
                perror("ls directory ");
            else 
            {
                int count =0;
                int total=0;
                while (count < k)
                {
                    char filepath[FILENAME_MAX];
                    strcpy(filepath,cmdarg[j]);
                    strcat(filepath,"/");
                    strcat(filepath,list[count]->d_name);
                    stat(filepath,&filestat);
                    total+=filestat.st_blocks;
                    count ++;
                }
                count =0;
                printf ("Total %d\n",total/2);
                while (count < k )
                {
                    char filepath1[FILENAME_MAX];
                    strcpy(filepath1,cmdarg[j]);
                    strcat(filepath1,"/");
                    strcat(filepath1,list[count]->d_name);
                    get_print_info(filepath1,list[count]->d_name);
                    free(list[count]);
                    count ++;
                }
                free (list);

            }
            }
        }
    }
}



void ls (char ** cmdarg)
{
    int flag=0;
    int aflag=0;
    int dirnamflag=0;
    int k;
    int lflag=0;
    int tocheck =1;
    struct dirent direntry;
    struct dirent ** list ;
    char directory[FILENAME_MAX];
    int hyparray[30]={0};
    int hypindex=0;
    for (int i=1;cmdarg[i]!=NULL;i++)
    {
        if (strcmp(cmdarg[i],"-l")==0 )
        {
            lflag++;
            hyparray[hypindex]=i;

        }
        else if  (strcmp(cmdarg[i],"-la")==0 ||strcmp(cmdarg[i],"-al")==0)
        {
            lflag++;
            aflag++;
        }
        else if (strcmp(cmdarg[i],"-a")==0)
        {
            aflag++;
        }
        else if (cmdarg[i][0]!='-')
        {
            dirnamflag++;
        }
            
    }

    if (lflag==0)
    {

        if (dirnamflag==0)
        {
            if (aflag)
                k=scandir(".",&list,NULL,alphasort);
            else 
                k=scandir(".",&list,filter,alphasort);

            int count =0;
            if (k < 0 )
                perror("ls directory ");
            else
            {
                while (count < k )
                {
                    stat(list[count]->d_name,&filestat);
                    printf("%s\n",list[count]->d_name);
                free(list[count]);
                count ++;
                }
            free (list);
            }
        }
        else 
        {
        for (int j=1;cmdarg[j]!=NULL ;j++)
        {
            if (cmdarg[j][0]!='-')
            {
            if (!aflag   && cmdarg[j][0]!='-')
                k=scandir(cmdarg[j],&list,filter,alphasort);
            else if (aflag && cmdarg[j][0]!='-' )
            {
                k=scandir(cmdarg[j],&list,NULL,alphasort);
            }
            int count =0;
            if (k < 0 )
                perror("ls directory ");

            else 
            {
                while (count < k )
                {
                    char filepath1[FILENAME_MAX];
                    strcpy(filepath1,cmdarg[j]);
                    strcat(filepath1,"/");
                    strcat(filepath1,list[count]->d_name);
                    stat(filepath1,&filestat);
                    printf("%s\n",list[count]->d_name);
                    free(list[count]);
                    count ++;
                }
                free (list);
            }
            }
        }
        }
    }
    else
    {
        lsl(aflag,cmdarg,dirnamflag);
    }    
}
