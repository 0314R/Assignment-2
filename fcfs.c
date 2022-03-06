#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "list.h"
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
	   readyAt = ioBusyUntil

	5. Add [readyAt, currentProcessIndex] to ordered list.

	The loop ends when there is nothing in the list anymore, i.e. NULL pointer.
*/
int DEFAULT_ARRAY_LENGTH = 10;

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
	int processCapability = DEFAULT_ARRAY_LENGTH, *startTimes, *finishTimes, **timesMatrix;
	char c;

	timesMatrix = malloc(processCapability * sizeof(int *));
	startTimes = malloc(processCapability * sizeof(int));
	finishTimes = malloc(processCapability * sizeof(int));

	int ignoredPriority, newNumber, i = 0, j, arraySize;

	do
	{
		if (i == processCapability)
		{
			processCapability *= 2;
			timesMatrix = realloc(timesMatrix, processCapability * sizeof(int *));
			startTimes = realloc(startTimes, processCapability * sizeof(int));
			finishTimes = realloc(finishTimes, processCapability * sizeof(int));
		}

		arraySize = DEFAULT_ARRAY_LENGTH;
		timesMatrix[i] = malloc(arraySize * sizeof(int));

		scanf("%d %d", &startTimes[i], &ignoredPriority); // the second int is priority and therefore ignored here.
		j = 0;

		do
		{
			if (j == arraySize)
			{
				arraySize *= 2;
				timesMatrix[i] = realloc(timesMatrix[i], arraySize * sizeof(int));
			}
			scanf("%d", &newNumber);
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

	// initialize variables
	int cpuBusyUntil = 0, ioBusyUntil = 0, r = 0;
	int *rowIdx = (int *)malloc(numberOfProcesses * sizeof(int));
	int *heap = (int *)malloc(2 * numberOfProcesses * sizeof(int));
	int heapIdx = 0, heapSize = 2 * numberOfProcesses;

	for (int i = 0; i < numberOfProcesses; i++)
	{
		heap[heapIdx] = startTimes[i];
		heap[heapIdx + 1] = i;
		heapIdx += 2;
		rowIdx[i] = 0;
	}

	// heapify heap array
	for (int i = heapIdx / 2 - 1; i >= 0; i -= 2)
		heapify(heap, heapIdx, i);

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

	int sum = 0;
	for (int i = 0; i < numberOfProcesses; i++)
	{
		sum += finishTimes[i] - startTimes[i];
		printf("%d ", finishTimes[i]);
		putchar('\n');
	}

	printf("%d\n", sum / numberOfProcesses);

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

	return 0;
}
