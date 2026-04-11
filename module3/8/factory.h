#ifndef FACTORY_H
#define FACTORY_H

#define STRING_SIZE 256

#include <stdlib.h>

//Вспомогательное
extern int c_wait;

void listener_SIGINT(int sig);

//Производитель
//Создание объекта
char *produce_item();
//Выгрузка объекта 
void put_item(char *filename);

//потребитель
//Забор объекта
void take_item();
//Обработка объекта
void consume_item();

#endif