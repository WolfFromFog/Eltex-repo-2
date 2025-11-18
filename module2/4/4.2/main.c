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
    insert(&pq, 5, "Всё норм 4.");

    print(&pq);

    priority = peekPriority(&pq);
    item = dequeue(&pq);

        // Dequeue an item and print it
    printf("С приоритетом %d элемент %s вышел из очереди \n", priority, item);
    // Print the top item of the queue
    printf("Верхний элемент %s с %d приоритетом\n", peek(&pq), peekPriority(&pq));
    // Dequeue an item and print it
    printf("С приоритетом %d элемент %s вышел из очереди \n", peekPriority(&pq), dequeue(&pq));
    // Print the top item of the queue
    printf("Верхний элемент %s с %d приоритетом\n", peek(&pq), peekPriority(&pq));
    // Dequeue an item and print it
    printf("С приоритетом %d элемент %s вышел из очереди \n", peekPriority(&pq), dequeue(&pq));
    // Print the top item of the queue
    printf("Верхний элемент %s с %d приоритетом\n", peek(&pq), peekPriority(&pq));



    return 0;
}