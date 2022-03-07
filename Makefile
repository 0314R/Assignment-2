program: fcfs.o heap.o
	gcc -o program fcfs.o heap.o

fcfs.o: fcfs.c heap.h
	gcc -c fcfs.c

heap.o: heap.c heap.h
	gcc -c heap.c
