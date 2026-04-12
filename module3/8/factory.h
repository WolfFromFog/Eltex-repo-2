#ifndef FACTORY_H
#define FACTORY_H

#define STRING_SIZE 256

#include <stdlib.h>

union semun
{
    int val;               /* значение для SETVAL */
    struct semid_ds *buf;  /* буферы для  IPC_STAT, IPC_SET */
    unsigned short *array; /* массивы для GETALL, SETALL */
                           /* часть, особенная для Linux: */
    struct seminfo *__buf; /* буфер для IPC_INFO */
};

// Вспомогательное
extern int c_wait;

void listener_SIGINT(int sig);

// Производитель
// Создание объекта
char *produce_item();
// Выгрузка объекта
ssize_t put_item(int filedisc, char *str);

// потребитель
// Забор объекта
ssize_t take_item(int filedisc, char *str);
// Обработка объекта
void consume_item(char *str);
void update_item(int filedisc);

#endif