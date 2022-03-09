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

typedef Queue* QueueSet;

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
QueueSet newQueueSet();


void freeQueue(Queue q);
void freeTripleQueue(TripleQueue tq);
void freeQueueSet(QueueSet set);

int isEmptyQueue(Queue q);
int isEmptyTQueue(TripleQueue q);
int isEmptyQueueSet(QueueSet qs);

void doubleQueueSize(Queue *qp);
void doubleTQueueSize(TripleQueue *qp);

void enqueue(Queue *qp, int item);
void enqueueT(TripleQueue *qp, int ready, int priority, int process);

int dequeue(Queue *qp);
void dequeueT(TripleQueue *qp, double *ready, int *priority, int *process);
int dequeueSet(QueueSet set);

double nextReadyAt(TripleQueue q);

void removeFromQueue(Queue *qp, int index);

void age(QueueSet cpuQs, double *ages, int *priorities, int lvl);

void printQueue(Queue q);
void printTQueue(TripleQueue q);
void printQueueSet(QueueSet set);
