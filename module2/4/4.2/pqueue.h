#ifndef PQUEUE_H
#define PQUEUE_H

#define MAX_PRIORITY 255
#define MAX_SIZE 100


//Структура очереди: размер, приоритет, элемент
typedef struct
{
    int size;
    int priority[MAX_SIZE];
    int item[MAX_SIZE];

} PriorityQueue;

//Помена двух значений
void swap(int *a, int *b);

//Функция, чтобы поддерживать баланс во время вставок
void heapifyUp(PriorityQueue *pq, int index);

//Вставка
void insert(PriorityQueue *pq, int priority, int value);

#endif