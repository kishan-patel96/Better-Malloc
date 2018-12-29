#ifndef _mymalloc_h_
#define _mymalloc_h_

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
#define MD_SIZE sizeof(md)


static char myblock[5000];

struct md { // structure definition for meta-data node that store information about a freeblock of memory
	size_t s;
	size_t free;
	struct md* next;
}md;

// names of our functions
void* mymalloc(size_t req, char *file, int line); // returns a void pointer to the beginning of a free chunk of memory if request is fulfilled
void myfree(void* p, char *file, int line); // frees memory allocated by mymalloc
void split(struct md* freeblock, size_t req); // splits a larger than needed free block of memory to more accurately satisfy an user request
void mergefree(); // merges free blocks/nodes of memory
int ismallocptr(void* p); // checks if a pointer was allocated by mymalloc
void wipeEntries(); // clears the linked list of meta-data nodes and resets the block of memory

#endif