#include "client.h"
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

int queue_connector()
{

    key_t key = ftok("./server", 'S');
    if (key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    int msqid = msgget(key, 0);
    if (msqid == -1)
    {
        perror("Ошибка: не удалось подключится к очереди.");
        exit(EXIT_FAILURE);
    }
    return msqid;
}
