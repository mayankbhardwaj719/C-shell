#include "headers.h"

void echo(char **token, int number_of_char)
{
    for (int i = 1; i < number_of_char - 1; i++)
    {
        printf("%s ", token[i]);
    }
    printf("%s", token[number_of_char - 1]);
    printf("\n");
    return;
}