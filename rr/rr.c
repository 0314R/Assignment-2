#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "heap.h"
#include "queue.h"

int max(int x, int y){
	return (x > y? x : y);
}

void printMatrix(double **matrix, int rows){
	printf("matrix:\n");
	for(int r=0 ; r<rows ; r++){
		int c=0;
		do{
			printf("%.0lf ", matrix[r][c]);
			c++;
		} while (matrix[r][c] != -1);
		putchar('\n');
	}
}

void freeMatrix(double **matrix, int rows){
	for(int i=0 ; i<rows ; i++){
		free(matrix[i]);
	}
	free(matrix);
}

int QUANTUM_LENGTH = 10;

int main(int argc, char *argv[])
{
	int *matrixRowIndexes, *priorities, processCapability = DEFAULT_ARRAY_LENGTH;
	double *startTimes, *finishTimes, *ages, **timesMatrix;

	timesMatrix = 	malloc(processCapability * sizeof(double *));
	startTimes = 	malloc(processCapability * sizeof(double));
	ages = 			malloc(processCapability * sizeof(double));
	priorities = 	malloc(processCapability * sizeof(int));

	int i = 0, j, arraySize;
	double newNumber;
	char c;

	do
	{
		if (i == processCapability)
		{
			processCapability *= 2;
			timesMatrix = realloc(timesMatrix, processCapability * sizeof(double *));
			startTimes =  realloc(startTimes, processCapability * sizeof(double));
			ages = 		  realloc(ages, processCapability * sizeof(double));
			priorities =  realloc(priorities, processCapability * sizeof(int));
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

	// It is a given that new processes arrive in the correct order, so no need for an ordered data structure, just a queue (with 3 fields).
	TripleQueue unstartedProcesses = newTQueue();
	double readyAt;
	int process, priority, numberOfProcesses = i, quantumRemainder = QUANTUM_LENGTH;

	finishTimes = malloc(numberOfProcesses * sizeof(double));
	matrixRowIndexes = malloc(numberOfProcesses * sizeof(int));

	// Loop over process indexes pi, to enqueue triples (ready, priority, process) into the TripleQueue.
	for (int pi = 0; pi < numberOfProcesses; pi++)
	{
		readyAt  = startTimes[pi];
		priority = priorities[pi];

		enqueueT(&unstartedProcesses, readyAt, priority, pi);
		matrixRowIndexes[pi] = 0;
	}

	// Set all ages to 0.
	for( int pi=0 ; pi < numberOfProcesses ; pi++){
		ages[pi] = 0;
	}

	printMatrix(timesMatrix, numberOfProcesses);
	printf("unstarted: ");
	printTQueue(unstartedProcesses);

	Queue ioQ = newQueue();
	QueueSet cpuQs = newQueueSet();
	int p, processUsingCPU=-1, processUsingIO=-1, t=0;
	double ioBusyUntil, cpuBusyUntil;


	while( processUsingIO!=-1 || processUsingCPU!=-1 || !isEmptyQueueSet(cpuQs) || !isEmptyQueue(ioQ) || !isEmptyTQueue(unstartedProcesses) ){
		// 2 Handle new, incoming process.
		while( !isEmptyTQueue(unstartedProcesses) && (t >= nextReadyAt(unstartedProcesses) ) ){
			dequeueT(&unstartedProcesses, &readyAt, &priority, &process);
			printf("removed [%.0lf, %d, %d] from Heap\n", readyAt, priority, process);
			enqueue(&cpuQs[priority], process);
			printf("[%d] (new) cpuQ%d: ", t, priority);
			printQueue(cpuQs[priority]);
		}

		// 3.1 Handle processes finishing an IO task.
		if( (processUsingIO != -1) && (t >= ioBusyUntil) ){
			p = processUsingIO;
			i = matrixRowIndexes[p];

			// If the process has no more CPU need, it is finished. Else it gets in a queue for the cpu.
			if( timesMatrix[p][i] == -1){
				finishTimes[p] = ioBusyUntil;
			} else {
				priority = priorities[p];
				enqueue(&cpuQs[priority], p);
				printf("[%d] (fIO) cpuQ%d: ", t, priority);
				printQueue(cpuQs[priority]);
			}
			processUsingIO = -1;

		}
		// 3.2 The IO can be used for a new process.
		if( processUsingIO == -1 && !isEmptyQueue(ioQ)){
			p = dequeue(&ioQ);
			i = matrixRowIndexes[p];

			processUsingIO = p;
			ioBusyUntil = t + timesMatrix[p][i];
			//printMatrix(timesMatrix, numberOfProcesses);
			//printf("matrixRowIndexes[%d] before update: %d\n", p, matrixRowIndexes[p]);
			matrixRowIndexes[p]++;

			printf("[%d] (sIO) ioBusyUntil: %.0lf ioQ: ", t, ioBusyUntil);
			printQueue(ioQ);
		}

		// 4 or 5, depending.
		if( (processUsingCPU != -1) && (t >= cpuBusyUntil) ){
			p = processUsingCPU;
			i = matrixRowIndexes[p];

			// 4 Handle processes finishing a CPU task
			if( timesMatrix[p][i] == 0 ){
				matrixRowIndexes[p]++;
				i = matrixRowIndexes[p];
				processUsingCPU = -1;
				// If the process has no more IO need, it is finished. Else it gets in the IO queue.
				if( timesMatrix[p][i] == -1){
					finishTimes[p] = cpuBusyUntil;
				} else {
					enqueue(&ioQ, p);
					printf("[%d] (fCPU) ioQ: ", t);
					printQueue(ioQ);
					printf("processUsingCPU = %d, isEmptyQueueSet = %d\n", processUsingCPU, isEmptyQueueSet(cpuQs));
					/* Redo loop iteration without updating t, so that if the IO is available, process p uses it now at time t instead of t+1.
					   So instead of doing step 6 this iteration, it is done in the next iteration, after step 3.2.*/
					continue;
				}

			}
			// 5 Since t >= cpuBusyUntil but the process is not done, we know the process was pre-emptively stopped because the quantum ended.

			else {
				p = processUsingCPU;
				priority = priorities[p];

				enqueue(&cpuQs[priority], p);
				printf("[%d] (fQT) cpuQ%d: ", t, priority);
				printQueue(cpuQs[priority]);
				processUsingCPU = -1;
			}
		}


		// 6 The CPU can be used for a new process; a new quantum is started.
		if( processUsingCPU == -1 && !isEmptyQueueSet(cpuQs)){
			p = dequeueSet(cpuQs);
			i = matrixRowIndexes[p];
			processUsingCPU = p;

			priority = priorities[p];
			if(priority >1 ) //reset the age
				ages[process] = 0;

			if(timesMatrix[p][i] <= QUANTUM_LENGTH){
				quantumRemainder = timesMatrix[p][i];
				timesMatrix[p][i] = 0;			//When the quantum finishes, we can look at this to say the process is done with CPU.
			} else {
				quantumRemainder = QUANTUM_LENGTH;
				timesMatrix[p][i] -= QUANTUM_LENGTH;
			}
			cpuBusyUntil = t + quantumRemainder;

			printf("[%d] (sCPU) timesMatrix[%d][%d] = %.0lf\n", t, p, i, timesMatrix[p][i]);
			printf("[%d]        cpuBusyUntil=%.0lf cpuQ%d:\n", t, cpuBusyUntil, priority);
			printQueue(cpuQs[priority]);
		}

		// 7 Age waiting processes
		i = cpuQs[2].front;
		while(i != cpuQs[2].back){
			process = cpuQs[2].arr[i];
			ages[process]++;
			i = (i+1) % cpuQs[2].size;
		}
		i = cpuQs[3].front;
		while(i != cpuQs[3].back){
			process = cpuQs[3].arr[i];
			ages[process]++;
			i = (i+1) % cpuQs[3].size;
		}
		// ages[processUsingIO]++;
		// i = ioQ.front;
		// while(i != ioQ.back){
		// 	process = ioQ.arr[i];
		// 	ages[process]++;
		// 	i = (i+1) % ioQ.size;
		// }
		printf("ages: ");
		for(i=0 ; i<numberOfProcesses ; i++){
			printf("%.0lf ", ages[i]);
		}
		putchar('\n');

		t++;
	}

	printf("finishTimes: ");
	for(p=0 ; p<numberOfProcesses ; p++)
		printf("%.0lf ", finishTimes[p]);
	putchar('\n');

	return 0;
}
