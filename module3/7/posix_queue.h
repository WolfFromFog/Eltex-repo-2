#ifndef POSIX_QUEUE_H
#define POSIX_QUEUE_H

#include <mqueue.h>

#define QUEUE_NAME_1 "/my_queue_1"
#define QUEUE_NAME_2 "/my_queue_2"
#define PRIORITY 1
#define SIZE 256
#define MSGSIZE 10

// Создание очереди
mqd_t create_queue(const char *queue_name);
// Открытие очереди
mqd_t open_queue(const char *queue_name);
// Откравка сообщения в очередь
mqd_t msg_send(mqd_t __mqdes, const char *__msg_ptr, size_t __msg_len, unsigned int __msg_prio);
// Получение сообщения из очереди
ssize_t msg_receive(mqd_t __mqdes, char *__msg_ptr, size_t __msg_len, unsigned int *__msg_prio);

#endif