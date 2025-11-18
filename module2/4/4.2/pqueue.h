#ifndef PQUEUE_H
#define PQUEUE_H

#define MAX_PRIORITY 255
#define MAX_SIZE 100


//Структура очереди: размер, приоритет, элемент. Очередь сделана через бинарное дерево
typedef struct
{
    int size;
    int priority[MAX_SIZE];
    char* item[MAX_SIZE];

} PriorityQueue;

//Помена двух значений
void swap(int *a, int *b);

//Функция, чтобы поддерживать баланс во время вставок
void heapifyUp(PriorityQueue *pq, int index);

//Вставка
void insert(PriorityQueue *pq, int priority, int value);

//Перебалнсировка во время удаления
void heapifyDown(PriorityQueue *pq, int index);

//Удаление из очереди первого элемента
int dequeue(PriorityQueue *pq);

//Удаление из очереди первого элемента с заданным приоритетом
int dequeueWithPriority(PriorityQueue *pq, int priority);

//Удаление из очереди первого элемента с приоритетом не ниже заданного
int dequeueNotLessPriority(PriorityQueue *pq, int priority);

//Просмотр первого элемента из очереди
int peek(PriorityQueue *pq);

//Просмотреть приоритет превого элемента в очереди
int peekPriority(PriorityQueue *pq);

//Просмотреть всю очередь
void print(PriorityQueue *pq);


#endif