2a: Scheduling - First Come First Serve

We decided to store the CPU times and IO times given in the input in a dynamic array of arrays,
because the number of arrays needed for processes is not given.

We decided to implement and use Heaps for keeping track of the ready processes in tuple form [ready, pro] (pro=process),
because a list would be too slow with allocations every time, plus a Heap is easy to use for sorting input
efficiently, or specifically not sorting but ensuring that the minimum element is at the root.

Program flow:
- Read inputs per line, the first number goes into the startTimes array, and after the priority which is ignored,
  and the rest of the line goes into a row of the matrix. The matrix is resized when needed, same for each of its rows.
- Insert all processes into the Heap, by key readyAt so that the next ready process is always at the root.
- While the heap is not empty (see number comments in loop):
  1. Remove the element at the Heap root (the next ready process);
  2. Read from the matrix what the next CPU burst is for this process and let it work the CPU for that amount of time;
  3. If the process is not finished (i.e. no -1 encountered in matrix), do the IO equivalent of step 2.
  4. If the process is not finished, move the index for the process' row in the matrix to the left by two.
  5. If the process is not finished, add it back to the heap again with the updated ready time after CPU and IO use.

__________________________________________________________________________________________________________________________

2a: Scheduling - Round-Robin with Priorities and Aging

As in FCFS, we decided to store the CPU times and IO times given in the input in a dynamic array of arrays,
because the number of arrays needed for processes is not given.

Unlike FCFS, we decided to implement and use several types of Queues instead of Heaps, most notably:
- A (Triple)Queue storing [ready, priority, process] triples for each not yet started process.
  Instead of using one array with triple nodes, it is implemented as three arrays which move together by index,
  because this way less time is spent on allocations.
- A QueueSet which is an array of 3 Queues, one for each CPU (technically 4 but we use indexes 1-3 only, to correspond to priority levels).
  The difference with the TripleQueue mentioned above is that the three Queues (arrays) do not move together for the QueueSet.
  The QueueSet groups the three priority queues together, so that we can implement high-level functions that (might) require all three
  priority queues, and (might) require them to reference each other, so that these things can be done in simple code (in main at least).

Program flow:
- Read inputs per line, the first number goes into the startTimes array, the second into the priorities array,
  and the rest of the line goes into a row of the matrix. The matrix is resized when needed, same for each of its rows.
- The not yet started processes' triples (ready, priority, process) are stored in unstartedProcesses, which is
  actually ordered by ready time because of the input structure.
- One queue ioQ is used for IO, and the three priority queues are used as a set in cpuQs.
- Then we loop while doing the 5 steps in the pdf, which are referenced in comments at appropriate points,
  except we added step 6 for starting a new (partial) quantum of use of the CPU,
  and step 7 for the aging, which is combined with step 1 from the pdf for promotions of waiting processes.
The comments in the code explain the flow in more detail.

__________________________________________________________________________________________________________________________

2b: Page Replacement

For both algorithms we chose to have an array representing the frames.
Then a while loop continues until a newline or EOF is read.
In each iteration the next number is read, after that we check whether
that number is currently in one of the frames. After that each
program deals with that according to the corresponding algorithm.

To check whether the page is already in one of the frames, the function
int find(int *arr, int len, int page) is used. This function checks whether
a page is already in one of the frames and returns the index of the array
where it is. If it is not in the array, it returns -1.

For this part of the lab, it was not necessary to use more complicated
data structures than just an array.
