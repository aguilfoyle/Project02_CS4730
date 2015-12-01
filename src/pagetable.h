//Invert to get victim frame
typedef struct
{
    int pid;
    int page;
} Invert;

//PageTableEntry
typedef struct
{
    int frameNo;//Physical page number
    bool valid; //Valid/invalid bit
    bool dirty; //Dirty bit
} PTE;

//Stats of hit / miss
typedef struct
{
    int hitCount;
    int missCount;
} STATS;

//PageTable
typedef struct
{
    PTE entry[MAX_PID][MAX_PAGE];
    STATS stats;
} PT;

int pageFaultHandler(int pid, int pageNo, char type, bool *hit);
extern int lruFlag;
extern int lfuFlag;
extern int frameIndex;
extern int frameReference;