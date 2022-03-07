#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int DEFAULT_ARRAY_LENGTH;

typedef struct Heap{
	int *ready;
	int *pro;		//process (index)
	int front;		//first free position
	int size;
} Heap;

Heap makeHeap();

int isEmptyHeap(Heap h);

void doubleHeapSize(Heap *hp);

void swap(int *a, int *b);

void upheap(Heap *hp, int i);

void downheap(Heap *hp, int i);

void enqueue(Heap *hp, int readyAt, int process);

void removeMin(Heap *hp, int *minReady, int *minProcess);

#endif
