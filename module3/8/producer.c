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
    // pid_t pid;
    // key_t key;
    // int semid;
    // union semun arg;
    int filedesc;
    filedesc = open(filename, O_WRONLY | O_CREAT, 0777);
    if (filedesc == -1)
    {
        printf("Не удалось открыть файл.\n");
        perror("Ошибка открытия");
        free(filename);
        exit(EXIT_FAILURE);
    }
    // struct sembuf lock_res = {0, -1, 0};
    // struct sembuf rel_res = {0, 1, 0};

    signal(SIGINT, listener_SIGINT);
    lseek(filedesc, 0, SEEK_END);
    while (c_wait)
    {
        char *item = produce_item();
        put_item(filedesc, item);
        sleep(1);
    }
    free(filename);
    close(filedesc);
    printf("Работа завершена.\n");
    return 0;
}