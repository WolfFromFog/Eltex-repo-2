#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        // char buffer[MSG_BUFF + 20];
        client_number = atol(argv[1]);
        int msqid = queue_connector();
        signal(SIGINT, listener_SIGINT);
        char msg2send[MSG_BUFF]; // msg2read[MSG_BUFF];
        do
        {
            msg_read(msqid);
            printf("Write message to send: ");
            if (fgets(msg2send, sizeof(msg2send), stdin) == NULL)
            {
                break;
            }
            msg2send[strcspn(msg2send, "\n")] = '\0';
            if (msg_send(msqid, msg2send) == -1)
            {
                printf("Ошибка отправки сообщения.\n");
                continue;
            }
            else
            {
                printf("Сообщение {%s} отправлено.\n", msg2send);
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