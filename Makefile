program: fcfs.o list.o
	gcc -o program fcfs.o list.o

fcfs.o: fcfs.c list.h
	gcc -c fcfs.c

list.o: list.c list.h
	gcc -c list.c