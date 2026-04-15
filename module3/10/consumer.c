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
    srand(10);
    char filename[] = "Makefile";
    key_t key = ftok(filename, 'F');
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }
    int semid, shmid;
    semid = semget(key, 1, 0666);
    if (semid == -1)
    {
        perror("semget (создание)");
        exit(1);
    }
    semctl(semid, 0, SETVAL, 1); // mutex = 1

    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1)
    {
        perror("shmget (создание)");
        semctl(semid, 0, IPC_RMID);
        exit(1);
    }

    // semctl(semid, 0, SETVAL, 1); // mutex = 1

    signal(SIGINT, listener_SIGINT);
    char buff[SHM_SIZE];
    int flag = 0;
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0}, {0, 1, 0}};
    char *shmaddr = shmat(shmid, NULL, 0);
    while (c_wait)
    {
        if (semop(semid, &lock, 1) == -1)
        {
            perror("semop:lock");
            exit(EXIT_FAILURE);
        }

        ssize_t bytes = take_item(shmaddr, buff);
        if (bytes < 0)
        {
            printf("Не удалось считать строку!\n");

            semop(semid, unlock, 2);
            continue;
        }
        if (bytes == 1)
        {
            if (flag == 0)
            {
                printf("Файл обработан. Ожидание новых данных.\n");
                flag = 1;
            }
            semop(semid, unlock, 2);
            sleep(1);
            continue;
        }
        if (strchr(shmaddr, 'и') == NULL)
        {
            consume_item(shmaddr, buff);
            flag = 0;
        }
        semop(semid, unlock, 2);
        sleep(1);
    }

    if (shmdt(shmaddr) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl (удаление)");
        exit(1);
    }
    printf("Удалён сегмент памяти (shmid=%d)\n", shmid);

    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("semctl (удаление)");
        exit(1);
    }
    printf("Удалён семафор (semid=%d)\n", semid);

    printf("Работа завершена.\n");
    return 0;
}