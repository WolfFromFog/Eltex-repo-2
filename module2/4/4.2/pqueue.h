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

//Помена двух значений приоритетов
void swapInt(int *a, int *b);

//Помена двух значений строк
void swapChar(char **a, char **b);

//Функция, чтобы поддерживать баланс во время вставок
void heapifyUp(PriorityQueue *pq, int index);

//Вставка
void insert(PriorityQueue *pq, int priority, char* value);

//Перебалнсировка во время удаления
void heapifyDown(PriorityQueue *pq, int index);

//Удаление из очереди первого элемента
char* dequeue(PriorityQueue *pq);

//Удаление из очереди первого элемента с заданным приоритетом
char* dequeueWithPriority(PriorityQueue *pq, int priority);

//Удаление из очереди первого элемента с приоритетом не ниже заданного
char* dequeueNotLessPriority(PriorityQueue *pq, int priority);

//Просмотр первого элемента из очереди
char* peek(PriorityQueue *pq);

//Просмотреть приоритет превого элемента в очереди
int peekPriority(PriorityQueue *pq);

//Просмотреть всю очередь
void print(PriorityQueue *pq);


#endif