#include "pqueue.h"
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue *pq, int index)
{
    if (index && pq->priority[(index - 1) / 2] > pq->priority[index])
    {
        swap(&pq->priority[(index - 1) / 2], &pq->priority[index]);
        swap(&pq->item[(index - 1) / 2], &pq->item[index]);
        heapifyUp(pq, (index - 1) / 2);
    }
}

void insert(PriorityQueue *pq, int priority, int value)
{
    if (pq->size == MAX_SIZE)
    {
        printf("Очередь полная!\n");
        return;
    }

    pq->priority[pq->size] = priority;
    pq->item[pq->size] = value;
    pq->size++;
    heapifyUp(pq, pq->size - 1);
}

void heapifyDown(PriorityQueue *pq, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->priority[left] < pq->priority[smallest])
        smallest = left;
    if (right < pq->size && pq->priority[right] < pq->priority[smallest])
        smallest = right;
    if (smallest != index)
    {
        swap(&pq->priority[index], &pq->priority[smallest]);
        swap(&pq->item[index], &pq->item[smallest]);
        heapifyDown(pq, smallest);
    }
}

int dequeue(PriorityQueue* pq)
{
    int item = pq->item[0];

    
    pq->priority[0] = pq->priority[pq->size-1];
    pq->item[0] = pq->item[pq->size-1];
    pq->size--;
    
    heapifyDown(pq, 0);
    return item;
}

int peek(PriorityQueue *pq)
{
    if (!pq->size)
    {
        printf("Priority queue is empty\n");
        return -1;
    }

    return pq->item[0];
}

int peekPriority(PriorityQueue *pq)
{
    if (!pq->size)
    {
        printf("Priority queue is empty\n");
        return -1;
    }

    return pq->priority[0];
}