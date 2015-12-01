#include <stdbool.h>

#define NDEBUG

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG(%s:%d) " M, __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define RANDOM 0
#define FIFO 1
#define LRU 2
#define LFU 3

#define MAX_PAGE 8 //4	//Org: 8
#define MAX_FRAME 3 //4	//Org: 3
#define MAX_PID 1 //4	//Org: 1

extern int replacementPolicy;

int page_replacement();
int memoryManagementUnit( int pid, int addr, char type, bool * hit );
void printPageTableStats();
void pageTableInit();
void printDiskStats();