#ifndef NETWORKER_H
#define NETWORKER_H

extern int nclients;

// Обработчек ошибок
void error(const char *msg);

// функция обслуживания
// подключившихся пользователей
void dostuff(int sock);
void printusers();
// функции
// сумма
int myadd(int a, int b);
// разность
int mysub(int a, int b);
// произведение
int mymult(int a, int b);
// частное
int mydiv(int a, int b);

union semun
{
    int val;               /* значение для SETVAL */
    struct semid_ds *buf;  /* буферы для  IPC_STAT, IPC_SET */
    unsigned short *array; /* массивы для GETALL, SETALL */
                           /* часть, особенная для Linux: */
    struct seminfo *__buf; /* буфер для IPC_INFO */
};

void signaler(int sig);

#endif