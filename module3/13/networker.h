#ifndef NETWORKER_H
#define NETWORKER_H

extern int nclients;

// Обработчек ошибок
void error(const char *msg);

// функция обслуживания
// подключившихся пользователей
void dostuff(int);
void printusers();
// функция обработки данных
int myfunc(int a, int b);

#endif