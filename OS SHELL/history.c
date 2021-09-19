#include "headers.h"



void addhis (char * cmd)
{
    strcpy(his.history[his.histins%20],cmd);
    his.histins++;    
}

void history(char ** cmd)
{
    int n;
    if (cmd[1]==NULL)
         n = 10;
    else 
         n= atoi(cmd[1]);
    for (int i=n;i>0;i--)
    {
        int k = his.histins-i;
        k=k%20;
        k=k+20;
        k=k%20;
        if (his.history[k]!=NULL && strlen(his.history[k])!=0)
        {
            printf("%s \n",his.history[k]);
        }
    }
}

void exhis()
{
    FILE * hfile;
    hfile=fopen(his_path,"wb");
    fwrite(&his, sizeof(struct hist), 1, hfile);
    fclose(hfile);
    exit(0);
}

void inithist()
{
    char currentdir [FILENAME_MAX];
    char temp[10];
    getcwd(currentdir,sizeof(currentdir));
    strcpy(temp,"/.history");
    strcat(currentdir,temp);
    strcpy(his_path,currentdir);
    FILE * hfile;
    hfile=fopen(his_path,"rb");
    if (hfile == NULL )
    {
        his.histins=0;
        his.histed=0;
    }
    else 
    {
        fread(&his, sizeof(struct hist), 1, hfile);
        fclose(hfile);
    }
}
