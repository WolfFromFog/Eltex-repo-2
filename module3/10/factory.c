#include "factory.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int c_wait = 1;

void listener_SIGINT(int sig)
{
    c_wait = 0;
}
// --------------------- Производитель
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
    size_t new_len = strlen(str) + 2;
    char *new_str = realloc(str, new_len + 1);
    if (!new_str)
    {
        free(str);
        return NULL;
    }
    new_str[new_len - 2] = ' ';
    new_str[new_len - 1] = '\n';
    new_str[new_len] = '\0';
    printf("Создана строка %s", new_str);
    return new_str;
}

ssize_t put_item(char *shm_block, char *str)
{
    if (str == NULL)
    {
        perror("Передана пустая строка");
        return -1;
    }

    size_t entered = strlen(str);
    ssize_t copied = 0;

    strcpy(shm_block, str);
    copied = strlen(shm_block);
    return (copied - entered) == 0 ? copied : -1;
}

//-----------------Потребитель

ssize_t take_item(char *shm_block, char *str)
{
    strcpy(str, shm_block);
    return strlen(str);
}

void consume_item(char *shm_block, char *str)
{
    int min = INT_MAX, max = INT_MIN;
    char *dup = malloc(strlen(str) + 1);
    strcpy(dup, str);
    char *token = strtok(dup, " \n");
    if (token == NULL)
    {
        perror("Пустая строка");
        printf("Пустая строка");
        return;
    }
    while (token != NULL)
    {

        if (token[0] != '\n')
        {
            int tmp = atoi(token);
            if (tmp > max)
            {
                max = tmp;
            }
            if (tmp < min)
            {
                min = tmp;
            }
        }
        token = strtok(NULL, " ");
    }
    printf("Минимум: %d, максимум: %d\n", min, max);
    snprintf(shm_block, SHM_SIZE, "Минимум: %d, максимум: %d\n", min, max);
    free(dup);
}
