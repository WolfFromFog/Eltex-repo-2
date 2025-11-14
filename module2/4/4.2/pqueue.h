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

//Перебалнсировка во время удаления
void heapifyDown(PriorityQueue *pq, int index);

//Удаление из очереди
int dequeue(PriorityQueue *pq);

//Просмотр первого элемента из очереди
int peek(PriorityQueue *pq);

//Просмотреть приоритет превого элемента в очереди
int peekPriority(PriorityQueue *pq);

#endif