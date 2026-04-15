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
    srand(10);
    char filename[] = "SMemory";

    sem_t *semap = sem_open(filename, O_CREAT, 0666, 1);
    if (semap == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }
    int shm_fd = shm_open(filename, O_RDWR, 0666);
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
    char buff[SHM_SIZE];
    int flag = 0;
    int counter = 0;
    while (c_wait)
    {
        // printf("Новый цикл\n");
        if (sem_wait(semap) == -1)
        {
            perror("sem_wait");
        }
        ssize_t bytes = take_item(shmaddr, buff);
        if (bytes < 0)
        {
            printf("Не удалось считать строку!\n");
            if (sem_post(semap) == -1)
            {
                perror("sem_post");
            }
            continue;
        }
        if (bytes == 0)
        {
            // printf("Пусто\n");
            if (flag == 0)
            {
                printf("Файл обработан. Ожидание новых данных.\n");
                flag = 1;
            }
            if (sem_post(semap) == -1)
            {
                perror("sem_post");
            }
            sleep(1);
            continue;
        }
        if (strchr(buff, 'и') == NULL)
        {
            // printf("Чтение\n");
            consume_item(shmaddr, buff);
            counter++;
            sleep(1);
            flag = 0;
        }
        if (sem_post(semap) == -1)
        {
            perror("sem_post");
        }
    }
    printf("Всего обработано: %d строк чисел.\n", counter);
    munmap(shmaddr, SHM_SIZE);
    sem_post(semap);
    sem_close(semap);
    printf("Работа завершена.\n");
    return 0;
}