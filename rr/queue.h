#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int DEFAULT_ARR_LEN;

typedef struct Queue{
	int *arr;
	int front;
	int back;
	int size;
} Queue;

Queue newQueue();

int isEmptyQueue(Queue q);

void doubleQueueSize(Queue *qp);

void enqueue(Queue *qp, int item);

int dequeue(Queue *qp);

void printQueue(Queue q);
