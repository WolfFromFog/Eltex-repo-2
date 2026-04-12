#include <stdlib.h>
#include <stdio.h>
#include "factory.h"
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

int main(int argc, char *argv[])
{
    srand(12);
    char *filename;
    if (argc == 2)
    {
        filename = (char *)malloc(strlen(argv[1]));
        strcpy(filename, argv[1]);
    }
    if (argc == 1)
    {
        char tmp_filename[] = "Storage";
        filename = (char *)malloc(strlen(tmp_filename));
        strcpy(filename, tmp_filename);
    }
    if (argc > 2)
    {
        printf("Ошибка. Неверное количество аргументов. Завершение работы.\n");
        perror("Wrong argc");
        exit(EXIT_FAILURE);
    }
    sem_t *semap = sem_open(filename, O_CREAT, 0666, 1);
    if (semap == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    int filedesc;
    filedesc = open(filename, O_WRONLY | O_CREAT, 0666);
    if (filedesc == -1)
    {
        printf("Не удалось открыть файл.\n");
        perror("Ошибка открытия");
        free(filename);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, listener_SIGINT);
    lseek(filedesc, 0, SEEK_END);
    while (c_wait)
    {
        char *item = produce_item();
        if (sem_wait(semap) == -1)
        {
            perror("sem_wait");
        }
        if (put_item(filedesc, item) < 0)
        {
            printf("Не удалось записать строку!\n");
        }
        if (sem_post(semap) == -1)
        {
            perror("sem_post");
        }
        sleep(1);
    }
    free(filename);
    close(filedesc);
    int val;
    sem_getvalue(semap, &val);
    if (val == 0)
        sem_post(semap);
    sem_close(semap);
    printf("Работа завершена.\n");
    return 0;
}