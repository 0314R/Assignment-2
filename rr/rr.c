#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "heap.h"
#include "queue.h"

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
	int processCapability = DEFAULT_ARRAY_LENGTH, *matrixRowIndexes, *priorities;
	double *startTimes, *finishTimes, **timesMatrix;

	timesMatrix = malloc(processCapability * sizeof(double *));
	startTimes = malloc(processCapability * sizeof(double));
	priorities = malloc(processCapability * sizeof(int));

	int i = 0, j, arraySize;
	double newNumber;
	char c;

	do
	{
		if (i == processCapability)
		{
			processCapability *= 2;
			timesMatrix = realloc(timesMatrix, processCapability * sizeof(double *));
			startTimes = realloc(startTimes, processCapability * sizeof(double));
			priorities = realloc(priorities, processCapability * sizeof(int));
		}

		arraySize = DEFAULT_ARRAY_LENGTH;
		timesMatrix[i] = malloc(arraySize * sizeof(double));
		scanf("%lf %d", &startTimes[i], &priorities[i]);
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

	Heap unstartedProcesses = makeHeap();
	double readyAt;
	int process, priority, numberOfProcesses = i;

	finishTimes = malloc(numberOfProcesses * sizeof(double));
	matrixRowIndexes = malloc(numberOfProcesses * sizeof(int));

	// Loop over process indexes pi, to enqueue triples (ready, priority, process) into the heap.
	for (int pi = 0; pi < numberOfProcesses; pi++)
	{
		readyAt  = startTimes[pi];
		priority = priorities[pi];
		insert(&unstartedProcesses, readyAt, priority, pi);
		matrixRowIndexes[pi] = 0;
	}

	printf("matrix:\n\n");
	for(int r=0 ; r<numberOfProcesses ; r++){
		int c=0;
		do{
			printf("%.0lf ", timesMatrix[r][c]);
			c++;
		} while (timesMatrix[r][c] != -1);
		putchar('\n');
	}
	printf("heap: ");
	printHeap(unstartedProcesses);

	Queue cpuQ1 = newQueue(), ioQ = newQueue();
	int p, processUsingCPU1=-1, processUsingIO=-1, t=0;
	double ioBusyUntil, cpuBusyUntil;



	while( !isEmptyQueue(cpuQ1) || !isEmptyHeap(unstartedProcesses) ){
		// 2 Handle new, incoming process.
		if( !isEmptyHeap(unstartedProcesses) && (t >= getMin(unstartedProcesses) ) ){
			removeMin(&unstartedProcesses, &readyAt, &priority, &process);
			printf("removed [%.0lf, %d, %d] from Heap\n", readyAt, priority, process);
			enqueue(&cpuQ1, process);
			printf("[%d] (2) cpuQ1: ", t);
			printQueue(cpuQ1);
		}

		// 3.1 Handle processes finishing an IO task.
		if( (processUsingIO != -1) && (t >= ioBusyUntil) ){
			p = processUsingIO;
			i = matrixRowIndexes[p];

			// If the process has no more CPU need, it is finished. Else it gets in a queue for the cpu.
			if( timesMatrix[p][i] == -1){
				finishTimes[p] = ioBusyUntil;
			} else {
				enqueue(&cpuQ1, p);
				printf("[%d] (3.1) cpuQ1: ", t);
				printQueue(cpuQ1);
			}
			processUsingIO = -1;

		}
		// 3.2 The IO can be used for a new process.
		if( processUsingIO == -1 && !isEmptyQueue(ioQ)){
			p = dequeue(&ioQ);
			i = matrixRowIndexes[p];

			processUsingIO = p;
			ioBusyUntil = t + timesMatrix[i][p];
			matrixRowIndexes[p]++;

			printf("[%d] (3.2) ioQ: ", t);
			printQueue(ioQ);
		}

		// 4.1 Handle processes finishing a CPU task.
		if( (processUsingCPU1 != -1) && (t >= cpuBusyUntil) ){
			p = processUsingCPU1;
			i = matrixRowIndexes[p];

			// If the process has no more IO need, it is finished. Else it gets in the IO queue.
			if( timesMatrix[p][i] == -1){
				finishTimes[p] = cpuBusyUntil;
			} else {
				enqueue(&ioQ, p);
				printf("[%d] (4.1) ioQ: ", t);
				printQueue(ioQ);
			}
			processUsingCPU1 = -1;

		}
		// 4.2 The CPU can be used for a new process.
		if( processUsingCPU1 == -1 && !isEmptyQueue(cpuQ1)){
			p = dequeue(&cpuQ1);
			i = matrixRowIndexes[p];

			processUsingCPU1 = p;
			cpuBusyUntil = t + timesMatrix[i][p];
			matrixRowIndexes[p]++;

			printf("[%d] (4.2) cpuQ1: ", t);
			printQueue(cpuQ1);
		}

		t++;
	}

	printf("finishTimes: ");
	for(p=0 ; p<numberOfProcesses ; p++)
		printf("%.0lf ", finishTimes[p]);
	putchar('\n');

	return 0;
}
