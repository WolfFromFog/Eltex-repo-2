#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "server.h"
#include <signal.h>

int main(int argc, char *argv[])
{
    int msqid = queue_creat();
    signal(SIGINT, listener_SIGINT);

    do
    {
        if (msg_read_test(msqid) == -1)
        {
            printf("Ошибка чтения сообщения из очереди. Удаление очереди\n");
            queue_delete(msqid);
            exit(EXIT_FAILURE);
        }
    } while (c_wait);

    queue_delete(msqid);
    return 0;
}