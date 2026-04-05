#include "server.h"
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// Флаг для сигнала
int c_wait = 1;

// Массив для хранения id клиентов
long int *clients = NULL;
int clients_count = 0;

// Создание очереди
int queue_creat()
{
    key_t key = ftok("./server", 'S');
    if (key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1)
    {
        perror("Ошибка: не удалось создать очередь.");
        exit(EXIT_FAILURE);
    }
    return msqid;
}

int msg_read(int msqid)
{
    my_msgbuf msg;
    if (msgrcv(msqid, &msg, MSG_BUFF, 10, 0) == -1)
    {
        perror("msgrcv");
        return -1;
    }
    printf("Получено сообщение: {%s}.\n", msg.mtext);

    char *space = strchr(msg.mtext, ' ');
    if (!space)
    {
        printf("Неверный формат сообщения\n");
        return -2;
    }
    *space = '\0';
    long int id = atol(msg.mtext);
    char *text = space + 1;

    if (strcmp(text, "is connecting") == 0)
    {
        addClient2Array(&clients, &clients_count, id);
        msg_send(msqid, "connected", id);
        return 0;
    }
    if (strcmp(text, "shutdown") == 0)
    {
        removeClientfromArray(&clients, &clients_count, id);
        msg_send(msqid, "disconnected", id);
        return 0;
    }

    msg_send(msqid, text, id);
    return 0;
}

int msg_read_test(int msqid)
{
    my_msgbuf msg;
    if (msgrcv(msqid, &msg, MSG_BUFF, 0, 0) == -1)
    {
        perror("msgrcv");
        printf("Ошибка чтения сообщения. msgrcv");
        return -1;
    }
    printf("Получено сообщение: {%s} приоритет {%ld}.\n", msg.mtext, msg.mtype);
    return 0;
}

int msg_send(int msqid, char *msg2send, long int mtype)
{
    my_msgbuf msg;
    msg.mtype = mtype;
    strcpy(msg.mtext, msg2send);
    int len = strlen(msg.mtext) + 1;
    if (msgsnd(msqid, &msg, len, 0) == -1)
    {
        perror("Не удалось отправить сообщение. msgsnd");
        return -1;
    }
    return 0;
}

// Удаление очереди
int queue_delete(int msqid)
{
    int status = msgctl(msqid, IPC_RMID, NULL);
    return status;
}

void listener_SIGINT(int sig)
{
    c_wait = 0;
}

void addClient2Array(long int **array, int *size, long int id)
{
    {
        (*size)++;
        long int *new_array = (long int *)realloc(*array, *size * sizeof(long int));
        if (!new_array)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        *array = new_array;
        (*array)[*size - 1] = id;
        printf("Добавлен клиент %ld.\n", id);
    }
}

void removeClientfromArray(long int **array, int *size, long int id)
{
    int flag = 0;
    for (int i = 0; i < *size; i++)
    {
        if ((*array)[i] == id)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 1)
    {
        if (*size - 1 == 0)
        {
            free(*array);
            *array = NULL;
        }
        else
        {
            long int *new_array = (long int *)malloc((*size - 1) * sizeof(long int));
            if (!new_array)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            for (int i = 0, j = 0; i < *size; i++)
            {
                if ((*array)[i] != id)
                {
                    new_array[j++] = (*array)[i];
                }
            }
            free(*array);
            *array = new_array;
        }
        (*size)--;
        printf("Клиент %ld удалён.\n", id);
    }
    else
    {
        printf("Такого клиента нет.\n");
    }
}
