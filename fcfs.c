#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "heap.h"

int max(int x, int y){
	return (x > y? x : y);
}

void freeMatrix(double **matrix, int rows){
	for(int i=0 ; i<rows ; i++){
		free(matrix[i]);
	}
	free(matrix);
}

int main(int argc, char *argv[])
{
	int processCapability = DEFAULT_ARRAY_LENGTH, *matrixRowIndexes;
	double *startTimes, *finishTimes, **timesMatrix;

	timesMatrix = malloc(processCapability * sizeof(double *));
	startTimes = malloc(processCapability * sizeof(double));

	int i = 0, j, arraySize;
	double newNumber;
	char c;

	// Read inputs into timesMatrix
	do
	{
		if (i == processCapability)
		{
			processCapability *= 2;
			timesMatrix = realloc(timesMatrix, processCapability * sizeof(double *));
			startTimes = realloc(startTimes, processCapability * sizeof(double));
		}

		//Each process has an array (row) in the matrix which contains its CPU and IO times from the input, plus the final -1.
		arraySize = DEFAULT_ARRAY_LENGTH;
		timesMatrix[i] = malloc(arraySize * sizeof(double));
		scanf("%lf %*d", &startTimes[i]); // priority is unneeded for fcfs, so we scan but ignore it.

		// Read a row of the matrix (i.e. rest of input line).
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
			break;
		else
			ungetc(c, stdin);

	} while (1);

	Heap h = makeHeap();
	double readyAt, cpuBusyUntil=0, ioBusyUntil=0;
	int process, ri, numberOfProcesses = i;

	finishTimes = malloc(numberOfProcesses * sizeof(double));
	matrixRowIndexes = malloc(numberOfProcesses * sizeof(int));

	// Loop over process indexes pi, to enqueue pairs of (readyAt, pi) into the heap.
	for (int pi = 0; pi < numberOfProcesses; pi++)
	{
		readyAt = startTimes[pi];
		enqueue(&h, readyAt, pi);
		matrixRowIndexes[pi] = 0;
	}

	while( !isEmptyHeap(h) ){
		/* 1. Read next ready process from list node [r, i] (=readyAt, processIndex)
	   		  Remove that node. */
		removeMin(&h, &readyAt, &process); 				//store return values using pointers

		//2
		ri = matrixRowIndexes[process];
		cpuBusyUntil = max(cpuBusyUntil, readyAt) + timesMatrix[process][ri];

		//3
		if( timesMatrix[process][ri+1] == -1){
			finishTimes[process] = cpuBusyUntil;
			continue;
		}
		ioBusyUntil = max(ioBusyUntil, cpuBusyUntil) + timesMatrix[process][ri+1];		//this is after.

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
		sum += finishTimes[i] - startTimes[i];
	printf("%.0lf\n", sum / numberOfProcesses);

	free(startTimes);
	free(finishTimes);
	free(matrixRowIndexes);

	freeMatrix(timesMatrix, numberOfProcesses);
	free(h.ready);
	free(h.pro);

	return 0;
}
