#include "server.h"
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

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

int msg_reader()
{
    return 0;
}

int msg_sender()
{
    return 0;
}
// Удаление очереди
int queue_delete(int msqid)
{
    int status = msgctl(msqid, IPC_RMID, NULL);
    return status;
}
