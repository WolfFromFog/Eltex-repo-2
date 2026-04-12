#include "posix_queue.h"
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>

mqd_t create_queue(const char *queue_name)
{
    mqd_t ds;
    struct mq_attr queue_attr;
    queue_attr.mq_msgsize = SIZE;
    queue_attr.mq_maxmsg = MSGSIZE;

    if ((ds = mq_open(queue_name, O_CREAT | O_RDWR, 0600, &queue_attr)) == (mqd_t)-1)
    {
        perror("Неудалось создать очередь");
        return -1;
    }

    return ds;
}

mqd_t open_queue(const char *queue_name)
{
    mqd_t ds;
    // struct mq_attr queue_attr;
    // unsigned int prior;
    if ((ds = mq_open(queue_name, O_RDWR, 0600, NULL)) == (mqd_t)-1)
    {
        perror("Неудалось открыть очередь");
        return -1;
    }
    return ds;
}

int msg_send(mqd_t __mqdes, const char *__msg_ptr, size_t __msg_len, unsigned int __msg_prio)
{
    printf("Идёт отправка сообщения: %s \n", __msg_ptr);
    if (mq_send(__mqdes, __msg_ptr, __msg_len, __msg_prio) == -1)
    {
        perror("Неудалось отправить сообщение в очередь");
        return -1;
    }
    printf("Сообщение отправлено. \n");
    return 0;
}

ssize_t msg_receive(mqd_t __mqdes, char *__msg_ptr, size_t __msg_len, unsigned int *__msg_prio)
{
    ssize_t res;
    printf("Вызван mq_receive, ждём...\n");
    if ((res = mq_receive(__mqdes, __msg_ptr, __msg_len, __msg_prio)) == -1)
    {
        perror("mq_receive");
        return res;
    }
    printf("mq_receive вернул %zd\n", res);
    return res;
}
