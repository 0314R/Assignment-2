#include "heap.h"

int DEFAULT_ARRAY_LENGTH = 10;

Heap makeHeap(){
	Heap h;

	h.ready = malloc(DEFAULT_ARRAY_LENGTH * sizeof(double));
	h.pro 	= malloc(DEFAULT_ARRAY_LENGTH * sizeof(int));
	assert(h.ready != NULL);
	assert(h.pro != NULL);

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
	hp->pro = realloc(hp->pro, hp->size * sizeof(int));
	assert(hp->ready != NULL);
	assert(hp->pro != NULL);
}

void swapInt(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void swapDouble(double *a, double *b){
	double temp = *a;
	*a = *b;
	*b = temp;
}

void upheap(Heap *hp, int i){
	if(i == 1)				// root of tree -> upheaping done
		return;

	int pi = i/2; 			//pi index = index/2
	if( hp->ready[i] < hp->ready[pi]){
		swapDouble( &(hp->ready[i]), &(hp->ready[pi]) );
		swapInt( &(hp->pro[i]), &(hp->pro[pi]) );
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
		swapDouble( &(hp->ready[lci]), &(hp->ready[i]) );
		swapInt( &(hp->pro[lci]), &(hp->pro[i]) );
		downheap(hp, lci);
	}
	else if ( hp->ready[rci] < hp->ready[i] ){
		swapDouble( &(hp->ready[rci]), &(hp->ready[i]) );
		swapInt( &(hp->pro[rci]), &(hp->pro[i]) );
		downheap(hp, rci);
	}
}

void enqueue(Heap *hp, double readyAt, int process){
	int fr = hp->front;
	if( fr == hp->size -1 ){
		doubleHeapSize(hp);
	}
	hp->ready[fr] = readyAt;
	hp->pro[fr] = process;

	upheap(hp, fr);
	hp->front = fr+1;
}

void removeMin(Heap *hp, double *minReady, int *minProcess){
	*minReady = hp->ready[1];
	*minProcess = hp->pro[1];
	hp->front--;
	hp->ready[1] = hp->ready[hp->front];
	hp->pro[1] 	 = hp->pro[hp->front];

	downheap(hp, 1);
}
