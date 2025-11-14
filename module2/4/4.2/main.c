#include "pqueue.h"
#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int priority, item;
    // I
    PriorityQueue pq = { 0 , {0} , {0}};
    // Add items to the queue
    insert(&pq, 3, 6);
    insert(&pq, 2, 88);
    insert(&pq, 15, 15);
    insert(&pq, 5, 16);
    insert(&pq, 4, 42);
    insert(&pq, 45, 0);

    priority = peekPriority(&pq);
    item = dequeue(&pq);

        // Dequeue an item and print it
    printf("С приоритетом %d элемент %d вышел из очереди \n", priority, item);
    // Print the top item of the queue
    printf("Верхний элемент %d с %d приоритетом\n", peek(&pq), peekPriority(&pq));
    // Dequeue an item and print it
    printf("С приоритетом %d элемент %d вышел из очереди \n", peekPriority(&pq), dequeue(&pq));
    // Print the top item of the queue
    printf("Верхний элемент %d с %d приоритетом\n", peek(&pq), peekPriority(&pq));
    // Dequeue an item and print it
    printf("С приоритетом %d элемент %d вышел из очереди \n", peekPriority(&pq), dequeue(&pq));
    // Print the top item of the queue
    printf("Верхний элемент %d с %d приоритетом\n", peek(&pq), peekPriority(&pq));

    return 0;
}