#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
	Ordered list with tuples [ready_at, process_index]
	e.g. [0,0] -> [15,1]

	Dynamic array of arrays. Row i contains the times of process i,
	except the first column tracks how much of the array has already been read.
	e.g.
	[[1, 100, 25, 100, 20, 50, 20, 100, 10, 200, -1],
	[1, 30, 15, 30, 10, 40, 10, 50, -1]]

	int numberOfProcesses
	Dynamic array finishTimes[numberOfProcesses] for the final calculation. (could just use a sum variable but easier to check this way if something goes wrong).
	Btw initialize that array with the start times of each.

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

int main(int argc, char *argv[]){
	int processCapability = DEFAULT_ARRAY_LENGTH, *startTimes, *finishTimes, **timesMatrix;

	timesMatrix = malloc(processCapability * sizeof(int*));
	startTimes = malloc(processCapability * sizeof(int));
	finishTimes = malloc(processCapability * sizeof(int));

	int newNumber, i=0, j, arraySize, *tempRowArray;

	do {
		if(i == processCapability){
			printf("resizing matrix from %d to %d rows.\n", processCapability, 2*processCapability);
			processCapability *= 2;
			timesMatrix = realloc( timesMatrix, processCapability * sizeof(int*));
			startTimes = realloc(startTimes, processCapability * sizeof(int));
			finishTimes = realloc(finishTimes, processCapability * sizeof(int));
		}

		arraySize = DEFAULT_ARRAY_LENGTH;
		tempRowArray   = malloc(arraySize * sizeof(int));
		timesMatrix[i] = malloc(arraySize * sizeof(int));

		scanf("%d %*d", &startTimes[i]); //the second int is priority and therefore ignored here.

		j=0;

		do{
			if(j==arraySize){
				printf(" <resize row array from %d to %d> ", arraySize, 2*arraySize);
				arraySize *= 2;
				tempRowArray   = realloc(tempRowArray, arraySize * sizeof(int));
				timesMatrix[i] = realloc(tempRowArray, arraySize * sizeof(int));
			}
			scanf("%d", &newNumber);
			tempRowArray[j] = newNumber;
			timesMatrix[i][j] = newNumber;
			printf("%d ", newNumber);
			j++;
		} while( newNumber != -1);

		putchar('\n');

		printf("entire row      : ");
		for(int k=0 ; k<j ; k++)
			printf("%d ", tempRowArray[k]);

		putchar('\n');

		printf("entire row again: ");
		for(int k=0 ; k<j ; k++)
			printf("%d ", timesMatrix[i][k]);

		putchar('\n');
		i++;

		// Now, at the end of the line after -1, check for EOF
		char c = getchar(); //ignore the newline
		printf("c = '%c'\n", c);
		if ( getchar() == EOF ){
			printf("EOF\n");
			break;
		}

	} while (1);

	printf("now time for the entire matrix\n\n");

	for(int r=0 ; r<i ; r++){
		int c=0;
		do{
			printf("%d ", timesMatrix[r][c]);
			c++;
		} while (timesMatrix[r][c] != -1);
		putchar('\n');
	}
	putchar('\n');


    return 0;
}
