#ifndef NETWORKER_H
#define NETWORKER_H

#include <sys/select.h>

extern int nclients;

extern fd_set master;

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

void set_nonblocking(int sock);

#define STATE_GET_OP 0   // ожидаем операцию (add/sum/mult/div)
#define STATE_GET_A 1    // ожидаем первое число
#define STATE_GET_B 2    // ожидаем второе число
#define STATE_SEND_RES 3 // отправляем результат
#define STATE_QUIT 4     // завершаем соединение

typedef struct
{
    int state;
    int a, b, opr;
    char buffer[1024];  // буфер на вход
    int buf_len;        // сколько байт уже в буфере
    char out_buf[1024]; // бфуер на выход
    int out_len;
    int out_sent; // сколько уже отправлено
} client_state_t;



#endif