#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int DEFAULT_ARRAY_LENGTH;

typedef struct Heap{
	double *ready;
	int *pro;		//process (index)
	int front;		//first free position
	int size;
} Heap;

Heap makeHeap();

int isEmptyHeap(Heap h);

void doubleHeapSize(Heap *hp);

void swapInHeap(Heap *hp, int i, int j);

void upheap(Heap *hp, int i);

void downheap(Heap *hp, int i);

void enqueue(Heap *hp, double readyAt, int process);

void removeMin(Heap *hp, double *minReady, int *minProcess);

#endif
