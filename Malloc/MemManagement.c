#include <stdio.h>
#include "MemEntry.h"

static char memory[5000];
#define BLOCKSIZE 5000

static struct MemEntry *root;
static int initialized = 0; 

void *myMalloc(int size, char *file, unsigned int line) {
	
	
	struct  MemEntry *p, *succ; 

	if(!initialized) {
		root=(struct MemEntry*) memory;
		root -> prev = root -> succ = 0;
		root -> size = BLOCKSIZE - sizeof(struct MemEntry);
		root -> isFree = 1; 
		initialized = 1;
	}
	p = root;
	do {
		if (p -> size < size)
			p = p -> succ; 
		else if (p -> isFree == 0)
			p = p -> succ; 
		else if (p -> size < (size + sizeof(struct MemEntry))) { 
			p -> isFree = 0;
			return (char *) p + sizeof(struct MemEntry); 
		}
		else {
			succ = (struct MemEntry*) ((char*) p + sizeof(struct MemEntry) + size);
			succ -> prev = p; 	
			succ -> succ = p -> succ;  
			if(p -> succ != 0) 
				p -> succ -> prev = succ;
			p -> succ = succ;
			succ -> size = p -> size - sizeof(struct MemEntry) - size;
			succ -> isFree = 1;
			p -> size = size; 
			p -> isFree = 0;
			return (char*) p + sizeof(struct MemEntry);	
		}
	} while(p != 0);
	
	printf("ERROR: NO MORE MEMORY, CANNOT MALLOC IN %s AT LINE %d\n", file, line);
	return 0;
}

int checkIsFree(struct MemEntry *ptr) {

	struct MemEntry *curr = root;
	while(curr != 0) {
		if(curr == ptr && curr -> isFree == 0)
			return 0;
		 curr = curr -> succ;
	}
	return 1;
}

void myFree(void *p, char *file, unsigned int line) {

	struct MemEntry *ptr, *pred, * succ;
	ptr = (struct MemEntry *)((char*)p - sizeof(struct MemEntry));
	if(checkIsFree(ptr)) {
		printf("ERROR: INVALID FREE IN %s AT LINE %d\n", file, line);
		return;
	}
 	pred = ptr -> prev;
	if (pred != 0 && pred -> isFree == 1) {
		pred -> size += sizeof(struct MemEntry) + ptr -> size;
		pred -> succ = ptr -> succ;
		if(ptr -> succ != 0)
			ptr -> succ -> prev = pred;
	}
	else {
		ptr -> isFree = 1;
		pred = ptr;
	}
	if ((succ = ptr -> succ) != 0 && succ -> isFree == 1) { 
		pred -> size += sizeof(struct MemEntry) + succ -> size;
		pred -> succ = succ -> succ;
		if(succ -> succ != 0) 
			succ -> succ -> prev = pred;
	}
}
