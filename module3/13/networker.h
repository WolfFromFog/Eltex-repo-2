#ifndef NETWORKER_H
#define NETWORKER_H

extern int nclients;

// Обработчек ошибок
void error(const char *msg);

// функция обслуживания
// подключившихся пользователей
void dostuff(int);
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

#endif