#include "posix_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    mqd_t ds_sender = (mqd_t)-1;
    mqd_t ds_reader = (mqd_t)-1;
    // int flag = 1;
    unsigned int prio;
    char msg[SIZE];
    int counter = 0;
    if (argc == 2)
    {
        printf("Пытаюсь создать очередь %s\n", QUEUE_NAME_1);
        if ((ds_sender = create_queue(QUEUE_NAME_1)) == -1)
        {
            printf("Не удалось создать очередь.\n");
            goto cleanup;
        }
        printf("Очередь %s открыта.\n", QUEUE_NAME_1);

        printf("Пытаюсь создать очередь %s\n", QUEUE_NAME_2);
        if ((ds_reader = create_queue(QUEUE_NAME_2)) == -1)
        {
            printf("Не удалось открыть очередь на чтение.\n");
            goto cleanup;
        }
        printf("Очередь %s открыта.\n", QUEUE_NAME_2);

        strncpy(msg, argv[1], SIZE - 1);
        msg[SIZE - 1] = '\0';
        while (counter < 30)
        {
            sleep(1);
            if (msg_send(ds_sender, msg, strlen(msg), PRIORITY) == -1)
            {
                printf("Не удалось отправить сообщение.\n");
                break;
            }
            ssize_t len = msg_receive(ds_reader, msg, SIZE, &prio);
            if (len == -1)
            {
                printf("Не удалось прочитать сообщение.\n");
                break;
            }
            msg[len] = '\0';
            printf("Получено сообщение со 2-го сервера: %s \n", msg);
            counter++;
        }
        /*

        if (mq_unlink(QUEUE_NAME_1) == -1)
        {
            perror("Неудалось удалить очередь 1");
        }
        if (mq_unlink(QUEUE_NAME_2) == -1)
        {
            perror("Неудалось удалить очередь 2");
        }
        */
    }
    else
    {
        if (argc == 1)
        {
            printf("Пытаюсь открыть очередь %s\n", QUEUE_NAME_1);
            if ((ds_reader = open_queue(QUEUE_NAME_1)) == -1)
            {
                printf("Не удалось открыть очередь 1 на чтение.\n");
                goto cleanup;
            }
            printf("Очередь %s открыта.\n", QUEUE_NAME_1);

            printf("Пытаюсь открыть очередь %s\n", QUEUE_NAME_2);
            if ((ds_sender = open_queue(QUEUE_NAME_2)) == -1)
            {
                printf("Не удалось открыть очередь 2 на чтение.\n");
                goto cleanup;
            }
            printf("Очередь %s открыта.\n", QUEUE_NAME_2);

            while (counter < 30)
            {
                sleep(1);
                ssize_t len = msg_receive(ds_reader, msg, SIZE, &prio);
                if (len == -1)
                {
                    printf("Не удалось прочитать сообщение.\n");
                    break;
                }
                msg[len] = '\0';
                printf("Получено сообщение со 1-го сервера: %s \n", msg);
                if (msg_send(ds_sender, msg, strlen(msg), PRIORITY) == -1)
                {
                    printf("Не удалось отправить сообщение.\n");
                    break;
                }
                counter++;
            }
        }
        else
        {
            printf("Максимум 1 аргумент.\n");
            goto cleanup;
        }
    }
cleanup:
    if (ds_sender != (mqd_t)-1)
    {
        if (mq_close(ds_sender) == -1)
        {
            perror("Неудалось закрыть очередь 1");
        }
    }
    if (ds_sender != (mqd_t)-1)
    {
        if (mq_close(ds_reader) == -1)
        {
            perror("Неудалось закрыть очередь 2");
        }
    }

    printf("Работа сервера завершена. \n");
    return 0;
}