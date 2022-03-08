#include "queue.h"

int DEFAULT_ARR_LEN = 10;

Queue newQueue(){
	Queue q;
	q.size = DEFAULT_ARR_LEN;
	q.arr = malloc(q.size * sizeof(int));
	assert(q.arr != NULL);

	q.front = 0;
	q.back = 0;
	return q;
}

int isEmptyQueue(Queue q){
	return q.back == q.front;
}

void doubleQueueSize(Queue *qp){
	int oldSize = qp->size;

	qp->size *= 2;
	qp->arr = realloc(qp->arr, (qp->size)*sizeof(int) );
	assert(qp->arr != NULL);

	//get rid of split configuration where qp->back  <  qp->front
	for(int i=0 ; i < qp->back ; i++)
		qp->arr[oldSize+i] = qp->arr[i];
	qp->back += oldSize;
}

void enqueue(Queue *qp, int item){
	qp->arr[qp->back] = item;
	qp->back = (qp->back+1) % qp->size;

	if( qp->back == qp->front)
		doubleQueueSize(qp);
}

int dequeue(Queue *qp){
	int item = qp->arr[qp->front];
	if(isEmptyQueue(*qp))
		printf("Error: trying to dequeue empty queue\n");

	qp->front = (qp->front+1) % qp->size;
	return item;
}

void printQueue(Queue q){
	for(int i=0 ; i<q.size ; i++){
		if(i==q.front) putchar('[');
		if(i==q.back) putchar(']');
		printf("%d ", q.arr[i]);
	}
	putchar('\n');
}
