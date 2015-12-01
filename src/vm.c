#include <stdio.h>
#include <time.h>
#include "vm.h"
#include "disk.h"

int replacementPolicy;

int main(int argc, char **argv)
{
    //*** VARIABLE(S) ***
    //Boolean(s)
    bool hit;
    //Integer(s)
    int pid;
    int addr;
    //Character(s)
    char type;
    char input[256];
    
    //Ensure random number generated each run
    srand(time(NULL));
    
    if(argc < 2)
    {
        fprintf(stderr, "usage: ./vm page_replacement_policy [0 - RANDOM, 1 - FIFO, 2 - LRU, 3 - LFU]\n");
        return -1;
    }
    
    //ReplacementPolicy for Part.A is 0; For Part.B it can be 0-3
    replacementPolicy = atoi(argv[1]);
    printf("Replacement Policy: %d - ", replacementPolicy);
    
    if(replacementPolicy == RANDOM)
    {
        printf("RANDOM\n");
    }
    else if(replacementPolicy == FIFO)
    {
        printf("FIFO\n");
    }
    else if(replacementPolicy == LRU)
    {
        printf("LRU\n");
    }
    else if(replacementPolicy == LFU)
    {
        printf("LFU\n");
    }
    else
    {
        printf("UNKNOWN!\n");
        return -1;
    }
    
    pageTableInit();
    
    //Main loop for program
    while( fgets( input, 256, stdin ))
    {
        if( sscanf( input, "%d %c 0x%x", &pid, &type, &addr ) < 3 )
        {
            printf("Error: invalid format\n");
            return 0;
        }
        
        int physicalAddr = memoryManagementUnit( pid, addr, type, &hit );
        
        if( hit )
        {
            printf( "Hit: [%d] %c 0x%x -> 0x%x\n", pid, type, addr, physicalAddr );
        }
        else
        {
            printf( "Miss: [%d] %c 0x%x -> 0x%x\n", pid, type, addr, physicalAddr );
        }
    }
    
    printf("=====================================\n");
    printPageTableStats();
    printDiskStats();
}