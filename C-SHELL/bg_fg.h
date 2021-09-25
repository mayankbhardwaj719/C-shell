#ifndef __BG_FG_H
#define __BG_FG_H


// typedef struct process
// {
//     int pid;
//     char name[2048];
// }process;

struct Node *ADDNode(struct Node *head, pid_t pid, char *name, int job_NUM);
struct Node *deleteNode(struct Node *head, pid_t pid);
char *findNode(struct Node *head, pid_t pid);
pid_t findNodebyJobNumber(struct Node *head, int job_NUM);

void bg_fg(char **token,int last,int k);
pid_t shellpid;

#endif
