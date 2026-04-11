#include <stdlib.h>
#include <stdio.h>
#include "producer.h"
#include <sys/sem.h>
#include <string.h>

char *produce_item()
{
    int count = rand() % 10 + 1;
    int arr[count];
    for (int i = 0; i < count; i++)
        arr[i] = rand();

    char buff[20];
    int len = sprintf(buff, "%d", arr[0]);
    char *str = malloc(len + 1);
    if (!str)
        return NULL;
    strcpy(str, buff);

    for (int i = 1; i < count; i++)
    {
        len = sprintf(buff, " %d", arr[i]);
        char *new_str = realloc(str, strlen(str) + len + 1);
        if (!new_str)
        {
            free(str);
            return NULL;
        }
        str = new_str;
        strcat(str, buff);
    }
    return str;
}

void put_item(char *filename)
{
    return;
}
