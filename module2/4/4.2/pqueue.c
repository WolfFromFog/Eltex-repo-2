#include "pqueue.h"
#include <stdio.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue* pq, int index)
{
    if(index && pq->priority[(index-1)/2] > pq->priority[index])
    {
        swap(&pq->priority[(index-1)/2], &pq->priority[index]);
        swap(&pq->item[(index-1)/2], &pq->item[index]);
        heapifyUp(pq, (index - 1) / 2);
    }
}

void insert(PriorityQueue* pq, int priority, int value)
{
    if(pq->size == MAX_SIZE)
    {
        printf("Очередь полная!\n");
        return;
    }

    pq->priority[pq->size++] = priority;
    pq->item[pq->size] = value;
    heapifyUp(pq, pq->size-1);
}