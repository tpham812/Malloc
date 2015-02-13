
#define malloc( x ) myMalloc( x, __FILE__, __LINE__ )
#define free( x ) myFree( x, __FILE__, __LINE__ )


struct MemEntry {

	struct MemEntry *prev;
	struct MemEntry *succ;
	unsigned int isFree;
	unsigned int size;	
};

void *myMalloc(int size, char *file, unsigned int line);
void myFree(void *p, char *file, unsigned int line);
