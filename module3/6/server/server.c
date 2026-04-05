#include "server.h"
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int c_wait = 1;

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

int msg_read()
{
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
    printf("Сообщение: {%s} приоритет {%d}.\n", msg.mtext, msg.mtype);
    return 0;
}

int msg_send()
{
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
