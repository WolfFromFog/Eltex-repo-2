#include "pqueue.h"
#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int priority;
    char* item;
    // I
    PriorityQueue pq = { 0 , {0} , {0}};
    // Add items to the queue
    insert(&pq, 0, "Всё норм 1.");
    insert(&pq, 255, "Диспечер мы падаем!");
    insert(&pq, 0, "Всё норм 2.");
    insert(&pq, 1, "Всё почти норм");
    insert(&pq, 0, "Всё норм 3.");
    insert(&pq, 5, "Есть проблемки");

    print(&pq);

    priority = peekPriority(&pq);
    item = dequeue(&pq);

    //Вывод из очереди верхнего элемента
    printf("С приоритетом %d элемент %s вышел из очереди \n", priority, item);
    // Проверка очереди
    print(&pq);

    
    item = dequeueWithPriority(&pq,0);

    // Вывод из очереди первого элемента с приоритетом 0
    printf("С приоритетом %d элемент %s вышел из очереди \n", 0, item);
    // Проверка очереди
    print(&pq);

    insert(&pq, 40, "Всё Плохо!");
    insert(&pq, 0, "Всё норм 4.");
    insert(&pq, 100, "ПЛОХА!");
    insert(&pq, 1, "Всё почти норм 2");
    insert(&pq, 0, "Всё норм 5.");
    insert(&pq, 20, "Есть проблемы!");

    printf("=============Вставлены новые элементы ==================\n");


    // Проверка очереди
    print(&pq);

    item = dequeueNotLessPriority(&pq, 15);

    // Вывод из очереди первого элемента с приоритетом не ниже 15
    printf("С приоритетом не ниже %d элемент %s вышел из очереди \n", 15, item);
    // Проверка очереди
    print(&pq);

    item = dequeueNotLessPriority(&pq, 15);

    // Вывод из очереди первого элемента с приоритетом не ниже 15
    printf("С приоритетом не ниже %d элемент %s вышел из очереди \n", 15, item);
    // Проверка очереди
    print(&pq);

    return 0;
}