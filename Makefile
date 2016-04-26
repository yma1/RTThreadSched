CC = gcc
CFLAGS = -lpthread -lm

SCHED: qsort.o main.o basicmath.o bitcount.o susan.o thread.o init.o
	$(CC) main.o qsort.o basicmath.o bitcount.o init.o susan.o thread.o -o $@ $(CFLAGS)

main.o:
	$(CC) -c main.c -o $@

qsort.o:
	$(CC) -c qsort.c -o $@ 

basicmath.o:
	$(CC) -c basicmath.c -o $@

bitcount.o:
	$(CC) -c bitcount.c -o $@

susan.o:
	$(CC) -c susan.c -o $@

thread.o:
	$(CC) -c thread.c -o $@

init.c:
	$(CC) -c init.c -o $@

clean:
	rm SCHED main.o qsort.o basicmath.o bitcount.o susan.o thread.o init.o
