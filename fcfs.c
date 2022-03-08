#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "heap.h"

/*
	Ordered data structure (e.g. priority queue) with tuples [ready_at, process_index]
	e.g. [[0,0], [15,1]] after initialization of 1.in

	Dynamic array of arrays: timesMatrix. Row i contains the times of process i.
	e.g.
	[[100, 25, 100, 20, 50, 20, 100, 10, 200, -1],
	[30, 15, 30, 10, 40, 10, 50, -1]]

	Array of row indexes to keep track of how much of each matrix row has been read.

	int numberOfProcesses
	Dynamic array finishTimes[numberOfProcesses] for the final calculation. (could just use a sum variable but easier to check this way if something goes wrong).

	int cpuBusyUntil, ioBusyUntil
	int readyAt, currentProcessIndex

	After reading inputs, loop these steps:
	1. Read next ready process from list node [r, i] (=readyAt, processIndex)
	   Remove that node.

	2. Let ri be the index for matrix row i,
	   cpuBusyUntil = max(cpuBusyUntil, readyAt)   <this is before adding the current process' CPU time.>
	   Add the cpu time at [i][ri] to cpuBusyUntil. <this is after adding the current process' CPU time.>

	3. IF there is no new IO but the process ends (-1),
	   THEN store cpuBusyUntil in index currentProcessIndex of the finish_times array, and start next loop iteration already.
	   ELSE
	   ioBusyUntil = max(ioBusyUntil, cpuBusyUntil) <this is before adding the current process' IO time.>
	   Add the io time at [i][ri+1] to ioBusyUntil <this is after adding the current process' IO time.>

	4. ri +=2;


	5. readyAt = ioBusyUntil
	   Add [readyAt, currentProcessIndex] to ordered list.

	The loop ends when there is nothing in the list anymore, i.e. NULL pointer.
*/

int max(int x, int y){
	return (x > y? x : y);
}

void printArrays(int *r, int *p, int length){
	for(int i=1 ; i<length ; i++)
		printf("[%d, %d] ", r[i], p[i]);
	putchar('\n');
}

int returnMax(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}

int main(int argc, char *argv[])
{
	int processCapability = DEFAULT_ARRAY_LENGTH, *matrixRowIndexes;
	double *startTimes, *finishTimes, **timesMatrix;
	char c;


	timesMatrix = malloc(processCapability * sizeof(double *));
	startTimes = malloc(processCapability * sizeof(double));
	finishTimes = malloc(processCapability * sizeof(double));
	matrixRowIndexes = malloc(processCapability * sizeof(int));

	int ignoredPriority, i = 0, j, arraySize;
	double newNumber;

	do
	{
		if (i == processCapability)
		{
			processCapability *= 2;
			timesMatrix = realloc(timesMatrix, processCapability * sizeof(double *));
			startTimes = realloc(startTimes, processCapability * sizeof(double));
			finishTimes = realloc(finishTimes, processCapability * sizeof(double));
			matrixRowIndexes = realloc(matrixRowIndexes, processCapability * sizeof(int));
		}

		arraySize = DEFAULT_ARRAY_LENGTH;
		timesMatrix[i] = malloc(arraySize * sizeof(double));

		scanf("%lf %d", &startTimes[i], &ignoredPriority); // the second int is priority and therefore ignored here.
		j = 0;

		do
		{
			if (j == arraySize)
			{
				arraySize *= 2;
				timesMatrix[i] = realloc(timesMatrix[i], arraySize * sizeof(double));
			}
			scanf("%lf", &newNumber);
			timesMatrix[i][j] = newNumber;
			j++;
		} while (newNumber != -1);
		i++;

		// Now, at the end of the line after -1, check for EOF
		getchar(); // ignore the newline
		c = getchar();
		if (!isdigit(c))
		{
			break;
		}
		else
		{
			ungetc(c, stdin);
		}

	} while (1);
	int numberOfProcesses = i;

/*
	printf("matrix:\n\n");
	for(int r=0 ; r<numberOfProcesses ; r++){
		int c=0;
		do{
			printf("%d ", timesMatrix[r][c]);
			c++;
		} while (timesMatrix[r][c] != -1);
		putchar('\n');
	}
*/
	Heap h = makeHeap();
	double readyAt, cpuBusyUntil=0, ioBusyUntil=0;
	int process, ri;

	// Loop over process indexes pi, to enqueue pairs of (readyAt, pi) into the heap.
	for (int pi = 0; pi < numberOfProcesses; pi++)
	{
		readyAt = startTimes[pi];
		enqueue(&h, readyAt, pi);
		matrixRowIndexes[pi] = 0;
	}

	//printArrays(h.ready, h.pro, h.front);

	while( !isEmptyHeap(h) ){
		/* 1. Read next ready process from list node [r, i] (=readyAt, processIndex)
	   		  Remove that node. */
		removeMin(&h, &readyAt, &process); 				//store return values using pointers

		//2
		ri = matrixRowIndexes[process];
		cpuBusyUntil = max(cpuBusyUntil, readyAt);		//this is before adding the current process' CPU time
		cpuBusyUntil += timesMatrix[process][ri];   	//this is after adding the current process' CPU time

		//3
		if( timesMatrix[process][ri+1] == -1){
			finishTimes[process] = cpuBusyUntil;
			continue;
		}
		ioBusyUntil = max(ioBusyUntil, cpuBusyUntil);	//this is before adding the current process' IO time.
		ioBusyUntil += timesMatrix[process][ri+1];		//this is after adding the current process' IO time.

		//4
		if( timesMatrix[process][ri+2] == -1){
			finishTimes[process] = ioBusyUntil;
			continue;
		}
		matrixRowIndexes[process] = ri+2;

		//5
		readyAt = ioBusyUntil;
		enqueue(&h, readyAt, process);
	}

	double sum = 0;
	for (int i = 0; i < numberOfProcesses; i++)
	{
		sum += finishTimes[i] - startTimes[i];
		//printf("%d ", finishTimes[i]);
		//putchar('\n');
	}

	printf("%.0lf\n", sum / numberOfProcesses);

/*

	while (heapIdx != 0)
	{
		// 1
		int readyAt = heap[0];
		int r = heap[1];

		if (heapIdx == 0)
			break;

		heap[0] = heap[heapIdx - 2]; // remove root node from heap
		heap[1] = heap[heapIdx - 1];
		heapIdx -= 2;
		for (int i = heapIdx / 2 - 1; i >= 0; i -= 2) // heapify again
			heapify(heap, heapIdx, i);

		// 2
		cpuBusyUntil = returnMax(cpuBusyUntil, readyAt);
		cpuBusyUntil += timesMatrix[r][rowIdx[r]];

		// 3
		if (timesMatrix[r][rowIdx[r] + 1] == -1)
		{
			finishTimes[r] = cpuBusyUntil;
			printf("process ended cpu\n");
			continue;
		}

		rowIdx[r] += 1;

		ioBusyUntil = returnMax(ioBusyUntil, cpuBusyUntil);
		ioBusyUntil += timesMatrix[r][rowIdx[r]];

		if (timesMatrix[r][rowIdx[r] + 1] == -1)
		{
			finishTimes[r] = ioBusyUntil;
			printf("process ended io\n");
			continue;
		}

		// 4
		rowIdx[r] += 1;
		readyAt = ioBusyUntil;

		// 5
		if (heapIdx + 2 > heapSize)
		{
			heapSize *= 2;
			heap = realloc(heap, heapSize * sizeof(int));
		}

		heapIdx += 2;
		heap[heapIdx - 2] = readyAt;
		heap[heapIdx - 1] = r;
		for (int i = heapIdx / 2 - 1; i >= 0; i -= 2) // heapify again
			heapify(heap, heapIdx, i);

		printf("loop: r %d, readyAt %d, cpu %d, io %d\n", r, readyAt, cpuBusyUntil, ioBusyUntil);
	}

	// freeList(list);
	free(heap);
	free(rowIdx);
	for (int i = 0; i < processCapability; i++)
	{
		free(timesMatrix[i]);
	}
	free(timesMatrix);
	free(finishTimes);
	free(startTimes);
*/
	return 0;
}
