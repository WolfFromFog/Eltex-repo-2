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
#include <sys/shm.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    srand(12);
    char filename[] = "Makefile";
    key_t key = ftok(filename, 'F');
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }

    semctl(semid, 0, SETVAL, 1); // mutex = 1

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0}, {0, 1, 0}};

    signal(SIGINT, listener_SIGINT);
    while (c_wait)
    {
        char *item = produce_item();
        if (semop(semid, &lock, 1) == -1)
        {
            perror("semop:lock");
        }
        char *shmaddr = shmat(shmid, NULL, 0);
        if (put_item(shmaddr, item) < 0)
        {
            printf("Не удалось записать строку!\n");
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("shmctl");
            exit(1);
        }
        semop(semid, unlock, 2);
        free(item);
        sleep(1);
    }
    printf("Работа завершена.\n");
    return 0;
}