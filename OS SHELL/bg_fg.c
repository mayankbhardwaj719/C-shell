#include "headers.h"


int error;


struct Node *ADDNode(struct Node *head, pid_t pid, char name[], int job_NUM)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    struct Node *start = head;

    new_node->pid = pid;
    strcpy(new_node->name, name);
    new_node->job = job_NUM;
    new_node->next = NULL;

    if (head==0)
    {
        head = new_node;
        return head;
    }

    while (start->next != NULL)
        start = start->next;

    start->next = new_node;
    return head;
}

struct Node *deleteNode(struct Node *head, pid_t pid)
{
    struct Node *temp = head;
    struct Node *start = head;

    if (temp && temp->pid == pid)
    {
        start = temp->next;
        free(temp);
        return start;
    }

    while (temp && temp->pid != pid)
    {
        start = temp;
        temp = temp->next;
    }

    start->next = temp->next;
    free(temp);
    return head;
}

char *findNode(struct Node *head, pid_t pid)
{
    if (head==0)
        return NULL;

    struct Node *temp = head;

    while (temp && temp->pid != pid)
        temp = temp->next;

    if (!temp)
        return NULL;

    return temp->name;
}

pid_t findNodebyJobNumber(struct Node *head, int job_NUM)
{
    if (head==0)
        return 0;

    struct Node *temp = head;

    while (temp && temp->job != job_NUM)
        temp = temp->next;

    if (temp==0)
        return 0;

    return temp->pid;
}



void bg_fg(char **token,int start,int k)
{

    int j = 0;
    for (int i = 0; i < k; i++)
    {
        if (strcmp(token[i], ""))
        {
            token[j++] = token[i];
        }
    }
    token[j] = 0;

    
    pid_t pid;
    int status;
    pid = fork();
    if (pid < 0)
    {
        perror("Failed forking");
        error = 1;
    }
    else if (!pid)
    {
        setpgid(0, 0);
        if (execvp(token[0], token) < 0)
        {
            fprintf(stderr, "Error executing %s command\n", token[0]);
            perror("");
            exit(1);
        }
    }
    else
    {
        dup2(shellOutFile, STDOUT_FILENO);
        dup2(shellInFile, STDIN_FILENO);
        if (start)
        {
            char name[1024];
            strcpy(name, token[0]);
            for (int i = 1; i < j; i++)
            {
                strcat(name, " ");
                strcat(name, token[i]);
            }

            struct Node *temp = head;
            int job_NUM = 1;
            while (temp != NULL)
            {
                job_NUM = temp->job + 1;
                temp = temp->next;
            }
            head = ADDNode(head, pid, name, job_NUM);
            printf("%d\n", pid);
        }
        else
        {
           
            tcsetpgrp(0, pid);
            waitpid(pid, &status, WUNTRACED);
            tcsetpgrp(0, shellpid);
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
            if (WIFSTOPPED(status))
            {
                error = 1;
                char name[2048];
                strcpy(name, token[0]);
                for (int i = 1; i < j; i++)
                {
                    strcat(name, " ");
                    strcat(name, token[i]);
                }

                struct Node *temp = head;
                int job_NUM = 1;
                while (temp != NULL)
                {
                    job_NUM = temp->job + 1;
                    temp = temp->next;
                }
                head = ADDNode(head, pid, name, job_NUM);
                printf("%s with PID %d suspended\n", name, pid);
            }

        }
    }
}
