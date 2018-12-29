all: memgrind

memgrind: mymalloc.o memgrind.o
	gcc -g -o memgrind mymalloc.o memgrind.o

mymalloc.o: mymalloc.c mymalloc.h
	gcc -g -c mymalloc.c

memgrind.o: memgrind.c
	gcc -g -c memgrind.c

clean:
	rm -f memgrind mymalloc.o memgrind.o