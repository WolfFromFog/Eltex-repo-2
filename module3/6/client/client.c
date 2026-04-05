#include "client.h"
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int c_wait = 1;
int client_number = 0;

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

int msg_send(int msqid, char *msg2send, int prior)
{
    // char str2send[MSG_BUFF + 4];
    //  sprintf(str2send, "%d %s", client_number, msg2send);
    my_msgbuf msg;
    msg.mtype = prior;
    strcpy(msg.mtext, msg2send);
    int len = strlen(msg.mtext) + 1;
    if (msgsnd(msqid, &msg, len, 0) == -1)
    {
        perror("Не удалось отправить сообщение. msgsnd");
        return -1;
    }
    return 0;
}

void listener_SIGINT(int sig)
{
    c_wait = 0;
}
