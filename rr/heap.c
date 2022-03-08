#include "heap.h"

int DEFAULT_ARRAY_LENGTH = 10;

Heap makeHeap(){
	Heap h;

	h.ready = malloc(DEFAULT_ARRAY_LENGTH * sizeof(double));
	h.process 	= malloc(DEFAULT_ARRAY_LENGTH * sizeof(int));
	h.priority 	= malloc(DEFAULT_ARRAY_LENGTH * sizeof(int));
	assert(h.ready != NULL);
	assert(h.process != NULL);

	h.front = 1;
	h.size = DEFAULT_ARRAY_LENGTH;

	return h;
}

int isEmptyHeap(Heap h){
	return h.front == 1;
}

void doubleHeapSize(Heap *hp){
	hp->size *= 2;

	hp->ready = realloc(hp->ready, hp->size * sizeof(double));
	hp->process = realloc(hp->process, hp->size * sizeof(int));
	hp->priority = realloc(hp->priority, hp->size * sizeof(int));
	assert(hp->ready != NULL);
	assert(hp->process != NULL);
	assert(hp->priority != NULL);
}

void swapInHeap(Heap *hp, int i, int j){
	double td; 		// temporary double
	int ti;			// temporary integer

	td = hp->ready[i];
	hp->ready[i] = hp->ready[j];
	hp->ready[j] = td;

	ti = hp->process[i];
	hp->process[i] = hp->process[j];
	hp->process[j] = ti;

	ti = hp->priority[i];
	hp->priority[i] = hp->priority[j];
	hp->priority[j] = ti;
}

void upheap(Heap *hp, int i){
	if(i == 1)				// root of tree -> upheaping done
		return;

	int pi = i/2; 			//pi index = index/2
	if( hp->ready[i] < hp->ready[pi]){
		swapInHeap(hp, i, pi);
		upheap(hp, pi);
	}
}

void downheap(Heap *hp, int i){
	if( i > (hp->front-1) /2 ) 	// leaf of a tree -> downheaping done
		return;

	int lci = 2*i;			// left child index
	int rci = 2*i + 1;		// right child index
	if( rci == hp->front )	// there is no right child
		rci = lci;			// this simplifies code below

	if( hp->ready[lci] < hp->ready[rci] && hp->ready[lci] < hp->ready[i]){
		swapInHeap(hp, i, lci);
		downheap(hp, lci);
	}
	else if ( hp->ready[rci] < hp->ready[i] ){
		swapInHeap(hp, i, rci);
		downheap(hp, rci);
	}
}

void insert(Heap *hp, double readyAt, int priority, int process){
	int fr = hp->front;
	if( fr == hp->size -1 ){
		doubleHeapSize(hp);
	}
	hp->ready[fr] = readyAt;
	hp->process[fr] = process;
	hp->priority[fr] = priority;

	upheap(hp, fr);
	hp->front = fr+1;
}

double getMin(Heap h){
	return h.ready[1];
}

void removeMin(Heap *hp, double *minReady, int *minPriority, int *minProcess){
	*minReady = hp->ready[1];
	*minProcess = hp->process[1];
	*minPriority = hp->priority[1];
	hp->front--;

	hp->ready[1] 	 = hp->ready[hp->front];
	hp->process[1]   = hp->process[hp->front];
	hp->priority[1]  = hp->priority[hp->front];

	downheap(hp, 1);
}

void printHeap(Heap h){
	for(int i=1 ; i<h.front ; i++)
		printf("[%.0lf, %d, %d] ", h.ready[i], h.priority[i], h.process[i]);
	putchar('\n');
}
