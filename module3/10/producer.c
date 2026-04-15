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
    key_t key = ftok(filename, 'F');
    int semid = semget(key, 2, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }
    int filedesc;
    filedesc = open(filename, O_WRONLY | O_CREAT, 0666);

    semctl(semid, 0, SETVAL, 1); // mutex = 1
    semctl(semid, 1, SETVAL, 0); // counter = 0
    if (filedesc == -1)
    {
        printf("Не удалось открыть файл.\n");
        perror("Ошибка открытия");
        free(filename);
        exit(EXIT_FAILURE);
    }
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};

    struct sembuf inc_counter = {1, +1, 0};
    struct sembuf dec_counter = {1, -1, SEM_UNDO}; // SEM_UNDO на случай падения
    // semop(semid, &inc_counter, 1);

    signal(SIGINT, listener_SIGINT);
    lseek(filedesc, 0, SEEK_END);
    while (c_wait)
    {
        char *item = produce_item();
        if (semop(semid, &lock, 1) == -1)
        {
            perror("semop:lock");
        }
        if (put_item(filedesc, item) < 0)
        {
            printf("Не удалось записать строку!\n");
        }
        semop(semid, &unlock, 1);
        sleep(1);
    }
    free(filename);
    close(filedesc);

    /*
    semop(semid, &dec_counter, 1);
    int cnt = semctl(semid, 1, GETVAL);
    if (cnt == 0)
    {
        semctl(semid, 0, IPC_RMID);
        printf("Семафор удалён\n");
    }
    */

    printf("Работа завершена.\n");
    return 0;
}