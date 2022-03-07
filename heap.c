#include "heap.h"

int DEFAULT_ARRAY_LENGTH = 10;

Heap makeHeap(){
	Heap h;

	h.ready = malloc(DEFAULT_ARRAY_LENGTH * sizeof(int));
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

	hp->ready = realloc(hp->ready, hp->size * sizeof(int));
	hp->pro = realloc(hp->pro, hp->size * sizeof(int));
	assert(hp->ready != NULL);
	assert(hp->pro != NULL);
}

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void upheap(Heap *hp, int i){
	if(i == 1)				// root of tree -> upheaping done
		return;

	int pi = i/2; 			//pi index = index/2
	if( hp->ready[i] < hp->ready[pi]){
		//printf("upheap swap %d= [%d, %d] %d= [%d, %d]\n", i, hp->ready[i], hp->pro[i], pi, hp->ready[pi], hp->pro[pi]);
		swap( &(hp->ready[i]), &(hp->ready[pi]) );
		swap( &(hp->pro[i]), &(hp->pro[pi]) );
		//printf("after  swap %d= [%d, %d] %d= [%d, %d]\n", i, hp->ready[i], hp->pro[i], pi, hp->ready[pi], hp->pro[pi]);
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
		////printf("downheap swap %d= [%d, %d] %d= [%d, %d]\n", i, hp->ready[i], hp->pro[i], lci, hp->ready[lci], hp->pro[lci]);
		swap( &(hp->ready[lci]), &(hp->ready[i]) );
		swap( &(hp->pro[lci]), &(hp->pro[i]) );
		////printf("after    swap %d= [%d, %d] %d= [%d, %d]\n", i, hp->ready[i], hp->pro[i], lci, hp->ready[lci], hp->pro[lci]);
		downheap(hp, lci);
	}
	else if ( hp->ready[rci] < hp->ready[i] ){
		////printf("downheap swap %d= [%d, %d] %d= [%d, %d]\n", i, hp->ready[i], hp->pro[i], rci, hp->ready[rci], hp->pro[rci]);
		swap( &(hp->ready[rci]), &(hp->ready[i]) );
		swap( &(hp->pro[rci]), &(hp->pro[i]) );
		////printf("after    swap %d= [%d, %d] %d= [%d, %d]\n", i, hp->ready[i], hp->pro[i], rci, hp->ready[rci], hp->pro[rci]);
		downheap(hp, rci);
	}
}

void enqueue(Heap *hp, int readyAt, int process){
	int fr = hp->front;
	//printf("COMPARING FR=%d with SIZE=%d\n", fr, hp->size);
	if( fr == hp->size -1 ){
		//printf("doubling heap size to %d\n", 2*hp->size);
		doubleHeapSize(hp);
	}

	//printf("right before enqueing into index %d\n", fr);
	//for(int i=1 ; i<hp->front ; i++)
		//printf("[%d, %d] ", hp->ready[i], hp->pro[i]);
	//putchar('\n');
	hp->ready[fr] = readyAt;
	hp->pro[fr] = process;
	//printf("right after  enqueing into index %d\n", fr);
	//for(int i=1 ; i<=hp->front ; i++)
		//printf("[%d, %d] ", hp->ready[i], hp->pro[i]);
	//putchar('\n');
	upheap(hp, fr);
	hp->front = fr+1;
}

void removeMin(Heap *hp, int *minReady, int *minProcess){
	*minReady = hp->ready[1];
	*minProcess = hp->pro[1];
	hp->front--;
	hp->ready[1] = hp->ready[hp->front];
	hp->pro[1] 	 = hp->pro[hp->front];

	downheap(hp, 1);
}
