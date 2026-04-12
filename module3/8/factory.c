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
    strcat(str, "\n");
    printf("Создана строка %s", str);
    return str;
}

ssize_t put_item(int filedisc, char *str)
{
    if (str == NULL)
    {
        perror("Передана пустая строка");
        return -1;
    }

    const char *p = str;
    size_t remaining = strlen(str);
    ssize_t total = 0;

    while (remaining > 0)
    {
        ssize_t written = write(filedisc, p, remaining);
        if (written < 0)
        {
            perror("Ошибка записи в файл");
            return -2;
        }
        if (written == 0)
        {
            perror("Ничего не записано в файл");
            return -3;
        }
        total += written;
        p += written;
        remaining -= written;
    }
    return total;
}

//-----------------Потребитель

ssize_t take_item(int filedisc, char *str)
{
    // lseek(filedisc, 0, SEEK_END);
    ssize_t bRead = read(filedisc, str, 1024);
    if (bRead < 0)
    {
        close(filedisc);
        return -1;
    }
    return bRead;
}

void consume_item(char *str)
{
    int min = INT_MAX, max = INT_MIN;
    char *token = strtok(str, " \n");
    if (token == NULL)
    {
        perror("Пустая строка");
        printf("Пустая строка");
        return;
    }
    while (token != NULL)
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
        token = strtok(NULL, " ");
    }
    printf("Минимум: %d, максимум: %d\n", min, max);
}

void update_item(int filedisc)
{
}
