#include "headers.h"


int error;


struct Node *insertNode(struct Node *head, pid_t pid, char name[], int jnum)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    struct Node *last = head;

    new_node->pid = pid;
    strcpy(new_node->name, name);
    new_node->job = jnum;
    new_node->next = NULL;

    if (!head)
    {
        head = new_node;
        return head;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
    return head;
}

struct Node *deleteNode(struct Node *head, pid_t pid)
{
    struct Node *temp = head;
    struct Node *last = head;

    if (temp && temp->pid == pid)
    {
        last = temp->next;
        free(temp);
        return last;
    }

    while (temp && temp->pid != pid)
    {
        last = temp;
        temp = temp->next;
    }

    last->next = temp->next;
    free(temp);
    return head;
}

char *findNode(struct Node *head, pid_t pid)
{
    if (!head)
        return NULL;

    struct Node *temp = head;

    while (temp && temp->pid != pid)
        temp = temp->next;

    if (!temp)
        return NULL;

    return temp->name;
}

pid_t findNodebyJobNumber(struct Node *head, int job)
{
    if (!head)
        return 0;

    struct Node *temp = head;

    while (temp && temp->job != job)
        temp = temp->next;

    if (!temp)
        return 0;

    return temp->pid;
}



void bg_fg(char **args,int last,int k)
{

    int j = 0;
    for (int i = 0; i < k; i++)
    {
        if (strcmp(args[i], ""))
        {
            args[j++] = args[i];
        }
    }
    args[j] = 0;

    
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
        if (execvp(args[0], args) < 0)
        {
            fprintf(stderr, "Error executing %s command\n", args[0]);
            perror("");
            exit(1);
        }
    }
    else
    {
        dup2(shellOutFile, STDOUT_FILENO);
        dup2(shellInFile, STDIN_FILENO);
        if (last)
        {
            char name[1024];
            strcpy(name, args[0]);
            for (int i = 1; i < j; i++)
            {
                strcat(name, " ");
                strcat(name, args[i]);
            }

            struct Node *temp = head;
            int jnum = 1;
            while (temp != NULL)
            {
                jnum = temp->job + 1;
                temp = temp->next;
            }
            head = insertNode(head, pid, name, jnum);
            printf("%d\n", pid);
        }
        else
        {
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            tcsetpgrp(0, pid);
            waitpid(pid, &status, WUNTRACED);
            tcsetpgrp(0, shellpid);
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
            if (WIFSTOPPED(status))
            {
                error = 1;
                char name[1024];
                strcpy(name, args[0]);
                for (int i = 1; i < j; i++)
                {
                    strcat(name, " ");
                    strcat(name, args[i]);
                }

                struct Node *temp = head;
                int jnum = 1;
                while (temp != NULL)
                {
                    jnum = temp->job + 1;
                    temp = temp->next;
                }
                head = insertNode(head, pid, name, jnum);
                printf("%s with PID %d suspended\n", name, pid);
            }

        }
    }
}
