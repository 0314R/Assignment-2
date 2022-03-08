#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int DEFAULT_ARRAY_LENGTH;

// Heap indexes start from 1, not 0. This simplifies parent index calculation.
typedef struct Heap{
	double *ready;
	int *process;
	int *priority;
	int front;			//first free position
	int size;
} Heap;

Heap makeHeap();

int isEmptyHeap(Heap h);

void doubleHeapSize(Heap *hp);

void swapInHeap(Heap *hp, int i, int j);

void upheap(Heap *hp, int i);

void downheap(Heap *hp, int i);

void insert(Heap *hp, double readyAt, int priority, int process);

double getMin(Heap h);

void removeMin(Heap *hp, double *minReady, int *minProcess, int *minPriority);

void printHeap(Heap h);

#endif
