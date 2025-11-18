#include "pqueue.h"
#include <stdio.h>
#include <stdlib.h>

void swapChar(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}

void swapInt(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue *pq, int index)
{
    if (index && pq->priority[(index - 1) / 2] < pq->priority[index])
    {
        swapInt(&pq->priority[(index - 1) / 2], &pq->priority[index]);
        swapChar(&pq->item[(index - 1) / 2], &pq->item[index]);
        heapifyUp(pq, (index - 1) / 2);
    }
}

void insert(PriorityQueue *pq, int priority, char* value)
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
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->priority[left] > pq->priority[largest])
        largest = left;
    if (right < pq->size && pq->priority[right] > pq->priority[largest])
        largest = right;
    if (largest != index)
    {
        swapInt(&pq->priority[index], &pq->priority[largest]);
        swapChar(&pq->item[index], &pq->item[largest]);
        heapifyDown(pq, largest);
    }
}

char* dequeue(PriorityQueue* pq)
{
    if (pq->size == 0)
    {
        printf("Очередь пуста!\n");
        return NULL;
    }
    char* item = pq->item[0];

    
    pq->priority[0] = pq->priority[pq->size-1];
    pq->item[0] = pq->item[pq->size-1];
    pq->size--;

    if (pq->size > 0)
    {
        heapifyDown(pq, 0);
    }
    return item;
}

char* dequeueWithPriority(PriorityQueue *pq, int priority)
{
    char* item;
    for(int i = 0; i < pq->size; i++)
    {
        if (pq->priority[i] == priority)
        {
            item = pq->item[i];
            pq->priority[i] = pq->priority[pq->size - 1];
            pq->item[i] = pq->item[pq->size - 1];
            pq->size--;

            if(i < pq->size)
            {
                heapifyUp(pq, i);
                heapifyDown(pq, i);
            }
            return item;
        }
    }
    return NULL;
}

char* dequeueNotLessPriority(PriorityQueue *pq, int priority)
{
    char* item;
    for (int i = 0; i < pq->size; i++)
    {
        if (pq->priority[i] >= priority)
        {
            item = pq->item[i];
            pq->priority[i] = pq->priority[pq->size - 1];
            pq->item[i] = pq->item[pq->size - 1];
            pq->size--;

            if (i < pq->size)
            {
                heapifyUp(pq, i);
                heapifyDown(pq, i);
            }
            return item;
        }
    }
    return NULL;
}

char* peek(PriorityQueue *pq)
{
    if (!pq->size)
    {
        printf("Priority queue is empty\n");
        return NULL;
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

void print(PriorityQueue *pq)
{

    if (pq->size == 0)
    {
        printf("Очередь пуста");
        return;
    }
    //Копия очереди
    int priorities[MAX_SIZE];
    char* items[MAX_SIZE];
    int size = pq->size;


    for (int i = 0; i < size; i++)
    {
        priorities[i] = pq->priority[i];
        items[i] = pq->item[i];
    }

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (priorities[j] < priorities[j + 1])
            {
                swapInt(&priorities[j], &priorities[j + 1]);
                swapChar(&items[j], &items[j + 1]);
            }
        }
    }
    
    for (int i = 0; i < size; i++)
    {
        printf("Элемент: %s, Приоритет: %d\n", items[i], priorities[i]);
    }
}

