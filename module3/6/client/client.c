#include "client.h"
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int c_wait = 1;
long int client_number;

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
    char msg2send[MSG_BUFF];
    sprintf(msg2send, "%ld is connecting", client_number);
    my_msgbuf msg;
    msg.mtype = 10;
    strcpy(msg.mtext, msg2send);
    int len = strlen(msg.mtext) + 1;
    if (msgsnd(msqid, &msg, len, 0) == -1)
    {
        perror("Ошибка: Не удалось отправить запрос подключения.");
        exit(EXIT_FAILURE);
    }
    return msqid;
}

int msg_send(int msqid, char *msg2send)
{
    char str2send[MSG_BUFF + 32];
    snprintf(str2send, sizeof(str2send), "%ld %s", client_number, msg2send);
    my_msgbuf msg;
    msg.mtype = 10;
    strcpy(msg.mtext, str2send);
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

int msg_read(int msqid)
{
    my_msgbuf msg;
    ssize_t ret = msgrcv(msqid, &msg, MSG_BUFF, client_number, IPC_NOWAIT);
    if (ret == -1)
    {
        if (errno == ENOMSG)
        {
            return 0;
        }

        perror("msgrcv");
        return -1;
    }
    else
    {
        printf("Получено сообщение {%s}\n", msg.mtext);
    }
    return 0;
}
