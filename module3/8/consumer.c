#include <stdlib.h>
#include <stdio.h>
#include "factory.h"
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    srand(10);
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
        free(filename);
        return 0;
    }
    int filedesc;
    filedesc = open(filename, O_RDWR | O_CREAT, 0777);
    if (filedesc == -1)
    {
        printf("Не удалось открыть файл.\n");
        perror("Ошибка открытия");
        free(filename);
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, listener_SIGINT);
    char buff[1024];
    int flag = 0;
    while (c_wait)
    {
        ssize_t bytes = take_item(filedesc, buff, sizeof(buff));
        if (bytes < 0)
        {
            printf("Не удалось считать строку!\n");
            continue;
        }
        if (bytes == 0)
        {
            if (flag == 0)
            {
                printf("Файл обработан. Ожидание новых данных.\n");
                flag = 1;
            }
            sleep(1);
            continue;
        }
        if (strchr(buff, '!') == NULL)
        {
            consume_item(buff);
            lseek(filedesc, -bytes, SEEK_CUR);
            buff[bytes - 2] = '!';
            write(filedesc, buff, bytes);
            flag = 0;
            sleep(1);
        }
    }
    free(filename);
    close(filedesc);
    printf("Работа завершена.\n");
    return 0;
}