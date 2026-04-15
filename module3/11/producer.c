#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include "factory.h"
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    srand(12);
    char filename[] = "SMemory";

    sem_t *semap = sem_open(filename, O_CREAT, 0666, 1);
    if (semap == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    int shm_fd = shm_open(filename, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("truncate");
        exit(EXIT_FAILURE);
    }

    char *shmaddr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shmaddr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    close(shm_fd);
    signal(SIGINT, listener_SIGINT);
    while (c_wait)
    {
        char *item = produce_item();
        if (sem_wait(semap) == -1)
        {
            perror("sem_wait");
        }
        if (put_item(shmaddr, item) < 0)
        {
            printf("Не удалось записать строку!\n");
        }
        if (sem_post(semap) == -1)
        {
            perror("sem_post");
        }
        sleep(1);

        if (sem_wait(semap) == -1)
        {
            perror("sem_wait");
        }
        if (strchr(shmaddr, 'и') != NULL)
        {
            printf("%s", shmaddr);
        }

        if (sem_post(semap) == -1)
        {
            perror("sem_post");
        }
        sleep(1);
    }
    munmap(shmaddr, SHM_SIZE);
    shm_unlink(filename);
    sem_post(semap);
    sem_close(semap);
    printf("Работа завершена.\n");
    return 0;
}