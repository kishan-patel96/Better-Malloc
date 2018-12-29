#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

struct md* mdlist = (void*)myblock; 

void* mymalloc(size_t req, char *file, int line) {

	// error if user tries allocating 0 bytes
	if (req == 0) { 
		printf("Can not allocate 0 bytes in FILE: '%s' on LINE: '%d'.\n", __FILE__, __LINE__);
		return 0;
	}

	// initializes the head node or block of memory 
	if (mdlist->s == 0) {
		mdlist->s = 5000 - sizeof(struct md);
		mdlist->free = 1;
		mdlist->next = NULL;
	}

	struct md* c = mdlist;
	
	// finds freeblock of memory
	while ((c->s < req || c->free == 0) && c->next != NULL) {
		c = c->next;
	}
	if (c->free == 1 && c->s == req) { // for exact fitting requests in freeblock of memory
		c->free = 0;
		return (void*) ++c;
	}
	else if (c->free == 1 && c->s > req + sizeof(struct md)) { // splits freeblock of memory if it exceeds user requirement + sizeof(struct md)
		split(c, req);
		return (void*) ++c;
	}
	else {	// final case where there isn't enough memory to fulfill the user's request
		printf("Insufficient memory to satisfy the requested amount of %zu bytes in FILE: '%s' on LINE: '%d'.\n", req, file, line);
		return NULL;
	}
}

void split(struct md* freeblock, size_t req) {

	struct md* temp = freeblock;
	int i = (int)temp->s;
	temp = temp->next;
	while (temp != NULL) {
		i = i + sizeof(struct md) + (int)temp->s;
		temp = temp->next;
	}
	size_t idx = 5000 - i; // finds index of freeblock

	struct md * rem = (void*)&myblock[idx + req + sizeof(struct md)]; // creates a new md node for the remaining chunk of free memory
	rem->s = 0;
	rem->s = freeblock->s - req - sizeof(struct md);
	rem->free = 1;
	freeblock->s = req;
	freeblock->free = 0;
	rem->next = freeblock->next;
	freeblock->next = rem;
}

void mergefree() {

	struct md* c = mdlist;
	while (c->next != NULL) { // loops through the list of md nodes and merges all free blocks including nodes
		if (c->free == 1 && c->next->free == 1) {
			c->s = c->s + c->next->s + sizeof(struct md);
			c->next = c->next->next;
			continue;
		}
		c = c->next;
	}
}

void myfree(void* p, char *file, int line) {

	if (p == NULL) {
		printf("Can't free null pointer in FILE: '%s' on LINE: '%d'.\n", file, line);
		return;
	}
	else if (!ismallocptr(p)) {
		printf("Can't free memory not allocated by malloc in FILE: '%s' on LINE: '%d'.\n", file, line);
		return;
	}

	if ((void*)&myblock[0] <= p && p < (void*)&myblock[4999]) {
		struct md* c = p;
		c--;
		if (c->free == 1) {
			printf("Memory allocated has already been freed in FILE: '%s' on LINE: '%d'.\n", file, line);
			return;
		}
		c->free = 1;
		mergefree();
	}
	else {
		printf("Can't free memory not allocated by malloc in FILE: '%s' on LINE: '%d'.\n", file, line);
	}
}

int ismallocptr(void* p) {
	struct md* ptr = p;
	ptr--;
	struct md* c = mdlist;
	while (c != NULL) {
		if (c == ptr) {
			return 1;
		}
		c = c->next;
	}
	return 0;
}

void wipeEntries() {
    mdlist = NULL;
}
