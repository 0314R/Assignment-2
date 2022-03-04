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

int main(int argc, char *argv[]){
	int processCapability = 10, *startTimes, *finishTimes, **timesMatrix;

	timesMatrix = malloc(processCapability * sizeof(int*));
	startTimes = malloc(processCapability * sizeof(int));
	finishTimes = malloc(processCapability * sizeof(int));

	int newNumber, i=0;

	do {
		scanf("%d %*d", &startTimes[i]); //the second int is priority and therefore ignored here.

		do{
			scanf("%d", &newNumber);
			printf("%d ", newNumber);
		} while( newNumber != -1);

		putchar('\n');
		// Now, at the end of the line after -1, check for EOF
		if ( feof(stdin) )
			break;

	} while (1);


    return 0;
}
