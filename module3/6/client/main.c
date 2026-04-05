#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        client_number = atoi(argv[1]);
        int msqid = queue_connector();
        signal(SIGINT, listener_SIGINT);
        char msg2send[MSG_BUFF], msg2read[MSG_BUFF];
        int msg_priority = 0;
        do
        {
            printf("Write message to send and to whom in format:{msg} {priority}");
            scanf("%s %d", msg2send, &msg_priority);
            if (msg_priority <= 0)
            {
                printf("Приоритет должен быть больше 0!\n");
                continue;
            }
            if (msg_send(msqid, msg2send, msg_priority) == -1)
            {
                printf("Ошибка отправки сообщения");
                continue;
            }
            else
            {
                printf("%d: Сообщение {%s} отправлено получателю {%d}\n", client_number, msg2send, msg_priority);
            }
        } while (c_wait);
    }
    else
    {
        perror("Неверное количество аргументов. Должен быть 1.");
        exit(EXIT_FAILURE);
    }
    return 0;
}