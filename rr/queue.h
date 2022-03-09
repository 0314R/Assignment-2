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

typedef struct TripleQueue{
	double *ready;
	int *priority;
	int *process;

	int front;
	int back;
	int size;
} TripleQueue;

Queue newQueue();
TripleQueue newTQueue();

int isEmptyQueue(Queue q);
int isEmptyTQueue(TripleQueue q);

void doubleQueueSize(Queue *qp);
void doubleTQueueSize(TripleQueue *qp);

void enqueue(Queue *qp, int item);
void enqueueT(TripleQueue *qp, int ready, int priority, int process);

int dequeue(Queue *qp);
void dequeueT(TripleQueue *qp, double *ready, int *priority, int *process);
double nextReadyAt(TripleQueue q);

void printQueue(Queue q);
void printTQueue(TripleQueue q);
