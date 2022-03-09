#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "queue.h"

int max(int x, int y){
	return (x > y? x : y);
}

int reachedEOF(){
	getchar(); // ignore the newline
	char c = getchar();
	if (!isdigit(c))
		return 1;

	//else put the character back.
	ungetc(c, stdin);
	return 0;
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
	int i=0, j, arraySize, *matrixRowIndexes, *priorities, processCapability = DEFAULT_ARR_LEN;
	double newNumber, *startTimes, *finishTimes, *ages, **timesMatrix;

	timesMatrix = 	malloc(processCapability * sizeof(double *));
	startTimes = 	malloc(processCapability * sizeof(double));
	priorities = 	malloc(processCapability * sizeof(int));

	// Read inputs into timesMatrix
	do
	{
		if (i == processCapability)
		{
			processCapability *= 2;
			timesMatrix = realloc(timesMatrix, processCapability * sizeof(double *));
			startTimes =  realloc(startTimes, processCapability * sizeof(double));
			priorities =  realloc(priorities, processCapability * sizeof(int));
		}

		//Each process has an array (row) in the matrix which contains its CPU and IO times from the input, plus the final -1.
		arraySize = DEFAULT_ARR_LEN;
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
		if(reachedEOF()) break;

	} while (1);

	// It is a given that new processes arrive in the correct order, so no need for an ordered data structure, just a queue (with 3 fields).
	TripleQueue unstartedProcesses = newTQueue();
	double readyAt, quantumRemainder = QUANTUM_LENGTH;
	int process, priority, numberOfProcesses = i;

	finishTimes = malloc(numberOfProcesses * sizeof(double));
	ages = malloc(numberOfProcesses * sizeof(double));
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

	Queue ioQ = newQueue();
	QueueSet cpuQs = newQueueSet();
	int p, processUsingCPU=-1, processUsingIO=-1, t=0;
	double ioBusyUntil, cpuBusyUntil;

	// The commented steps below correspond to those of the pdf, but since we do the pdf's step 1 at the end, it starts with 2 here.
	while( processUsingIO!=-1 || processUsingCPU!=-1 || !isEmptyQueueSet(cpuQs) || !isEmptyQueue(ioQ) || !isEmptyTQueue(unstartedProcesses) ){
		// 2 Handle new, incoming process.
		while( !isEmptyTQueue(unstartedProcesses) && (t >= nextReadyAt(unstartedProcesses) ) ){
			dequeueT(&unstartedProcesses, &readyAt, &priority, &process);
			enqueue(&cpuQs[priority], process);
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
			}
			processUsingIO = -1;

		}
		// 3.2 The IO can be used for a new process.
		if( processUsingIO == -1 && !isEmptyQueue(ioQ)){
			p = dequeue(&ioQ);
			i = matrixRowIndexes[p];

			processUsingIO = p;
			ioBusyUntil = t + timesMatrix[p][i];
			matrixRowIndexes[p]++;
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
		}

		// 7 Age waiting processes and 8 (=1 in the lab pdf) Promote waiting processes to higher priority.
		age(cpuQs, ages, priorities, 2);
		age(cpuQs, ages, priorities, 3);

		t++;
	}

	//Compute the final answer
	double sum=0;
	for(p=0 ; p<numberOfProcesses ; p++){
		sum += finishTimes[p] - startTimes[p];
	}
	printf("%.0lf\n", sum / (double)numberOfProcesses);

	free(matrixRowIndexes); free(priorities); free(startTimes); free(finishTimes); free(ages);
	freeMatrix(timesMatrix, numberOfProcesses); free(ioQ.arr); freeQueueSet(cpuQs); freeTQueue(&unstartedProcesses);

	return 0;
}
