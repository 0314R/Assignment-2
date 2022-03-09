#include "queue.h"

int DEFAULT_ARR_LEN = 4;

Queue newQueue(){
	Queue q;
	q.size = DEFAULT_ARR_LEN;
	q.arr = malloc(q.size * sizeof(int));
	assert(q.arr != NULL);

	q.front = 0;
	q.back = 0;
	return q;
}

TripleQueue newTQueue(){
	TripleQueue q;
	q.size = DEFAULT_ARR_LEN;

	q.ready = malloc(q.size * sizeof(double));
	q.priority = malloc(q.size * sizeof(int));
	q.process = malloc(q.size * sizeof(int));
	assert(q.ready != NULL);
	assert(q.priority != NULL);
	assert(q.process != NULL);

	q.front = 0;
	q.back = 0;
	return q;
}

QueueSet newQueueSet(){
	QueueSet qs;
	qs = malloc(4 * sizeof(Queue));
	for( int priority=1 ; priority<=3 ; priority++){
		qs[priority] = newQueue();
	}
	return qs;
}

int isEmptyQueue(Queue q){
	return q.back == q.front;
}

int isEmptyTQueue(TripleQueue q){
	return q.back == q.front;
}

int isEmptyQueueSet(QueueSet qs){
	return isEmptyQueue(qs[1]) && isEmptyQueue(qs[2]) && isEmptyQueue(qs[3]);
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

void doubleTQueueSize(TripleQueue *qp){
	int oldSize = qp->size;

	qp->size *= 2;
	qp->ready = realloc(qp->ready, (qp->size)*sizeof(double) );
	qp->priority = realloc(qp->priority, (qp->size)*sizeof(int) );
	qp->process = realloc(qp->process, (qp->size)*sizeof(int) );
	assert(qp->ready != NULL);
	assert(qp->priority != NULL);
	assert(qp->process != NULL);

	//get rid of split configuration where qp->back  <  qp->front
	for(int i=0 ; i < qp->back ; i++){
		qp->ready[oldSize+i] = qp->ready[i];
		qp->process[oldSize+i] = qp->process[i];
		qp->priority[oldSize+i] = qp->priority[i];
	}
	qp->back += oldSize;
}

void enqueue(Queue *qp, int item){
	qp->arr[qp->back] = item;
	qp->back = (qp->back+1) % qp->size;

	if( qp->back == qp->front)
		doubleQueueSize(qp);
}

void enqueueT(TripleQueue *qp, int ready, int priority, int process){
	qp->ready[qp->back] = ready;
	qp->priority[qp->back] = priority;
	qp->process[qp->back] = process;

	qp->back = (qp->back+1) % qp->size;
	if( qp->back == qp->front)
		doubleTQueueSize(qp);
}

int dequeue(Queue *qp){
	int item = qp->arr[qp->front];
	if(isEmptyQueue(*qp))
		printf("Error: trying to dequeue empty queue\n");

	qp->front = (qp->front+1) % qp->size;
	return item;
}

void dequeueT(TripleQueue *qp, double *ready, int *priority, int *process){
	if(isEmptyTQueue(*qp))
		printf("Error: trying to dequeue empty queue\n");

	*ready = qp->ready[qp->front];
	*priority = qp->priority[qp->front];
	*process = qp->process[qp->front];
	qp->front = (qp->front+1) % qp->size;
}

int dequeueSet(QueueSet set){
	int priority=1;
	if(isEmptyQueueSet(set))
		printf("ERROR: TRYING TO DEQUEUE FROM EMPTY SET\n");

	while(isEmptyQueue(set[priority]))
		priority++;

	//printf("dequeued from priority %d\n", priority);
	return dequeue(&set[priority]);
}

double nextReadyAt(TripleQueue q){
	if(isEmptyTQueue(q))
		printf("Error: trying to read from empty queue\n");

	return q.ready[q.front];
}

void removeFromQueue(Queue *qp, int index){
	//Shift all elements one to the left from index to back
	// printf("trying to remove index %d from this queue: ", index);
	// printQueue(*qp);
	int nextIndex = (index+1) % qp->size;
	//printf("nextIndex: %d, size=%d, 4%%4==%d\n", nextIndex, qp->size, 4%4);
	while( nextIndex != qp->back ){
		qp->arr[index] = qp->arr[nextIndex];
		index = nextIndex;
		nextIndex = (index+1) % qp->size;
	}
	qp->back = (qp->back == 0 ? qp->size-1 : qp->back-1);
}

void printQueue(Queue q){
	int i = q.front;
	putchar('[');
	//printf("[%d] ", i);
	while(i != q.back){
		printf("%d ", q.arr[i]);
		i = (i+1) % q.size;
	}
	printf("]\n");
	//printf("[%d] = [%d] = q.back, q.size=%d\n", i, q.back, q.size);
	// if(isEmptyQueue(q))
	// 	printf("(empty) ");
	//
	// for(int i=0 ; i<q.size ; i++){
	// 	if(i==q.front) putchar('[');
	// 	if(i==q.back) putchar(']');
	// 	printf("%d ", q.arr[i]);
	// }
	// putchar('\n');
}

void printTQueue(TripleQueue q){
	int i = q.front;
	while(i != q.back){
		printf("[%.0lf, %d, %d] ", q.ready[i], q.priority[i], q.process[i]);
		i = (i+1) % q.size;
	}
	putchar('\n');
}

void printQueueSet(QueueSet set){
	for(int q=1 ; q<=3 ; q++) {
		printf("cpuQ%d: ", q);
		printQueue(set[q]);
	}
}
