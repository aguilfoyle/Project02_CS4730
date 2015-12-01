//---INCLUDED---
#include <stdio.h>
#include "vm.h"
#include "disk.h"
#include "pagetable.h"



//---GLOBAL VARIABLE(S)---
int lruFlag;
int lfuFlag;
int frameIndex;
int frameReference;
PT pageTable;
Invert invert_table[MAX_FRAME];
//---DECLARED METHODS---
int hitTest( int pid, int pageNo, char type );
int pageFaultHandler(int pid, int pageNo, char type, bool *hit);
int memoryManagementUnit(int pid, int addr, char type, bool *hit);
void printPageTableStats();
void pageTableInit();


/*
 * NAME: hitTest() | PARAM: int, int, char | RETURN: int
 * PURPOSE: Provided by professor Less
 */
int hitTest(int pid, int pageNo, char type)
{
    int frameNo;
    lruFlag = 2; //Set lruFlag for hit to 2
    lfuFlag = 2; //Set lfuFlag for hit to 2
    
    if(pageTable.entry[pid][pageNo].valid == false) //If there was a miss
    {
        //Increment miss count
        pageTable.stats.missCount = pageTable.stats.missCount + 1;
        
        return -1;
    }

    //Increment hit count
    pageTable.stats.hitCount = pageTable.stats.hitCount + 1;
    
    if( replacementPolicy == 2 || replacementPolicy == 3 )
    {
        frameReference = pageTable.entry[pid][pageNo].frameNo;
        page_replacement();
    }
    
    if(type == 'W')
    {
    	pageTable.entry[invert_table[pageTable.entry[pid][pageNo].frameNo].pid][invert_table[pageTable.entry[pid][pageNo].frameNo].page].dirty = true;
    }
    
    return pageTable.entry[pid][pageNo].frameNo;
}



/*
 * NAME: pageFaultHandler() | PARAM: int, int, char | RETURN: int
 * PURPOSE: Provided by professor Less
 */
int pageFaultHandler(int pid, int pageNo, char type, bool *hit)
{
    //Hit is false since pageFaultHandler is being called
    *hit = false;

    //Check for free frame
    if(frameIndex == MAX_FRAME) //FULL
    {
        //CALL REPLACEMENT ALGORITHUM
        lruFlag = 1; //Set lruFlag for full to 1
        lfuFlag = 1; //Set lfuFlag for full to 1
        int victimFrame = page_replacement();
        int victimPID = invert_table[victimFrame].pid;
        int victimPageNo = invert_table[victimFrame].page;
        
        //***ASSIGN NEW VALID, & CHECK DIRTY FOR VICTIM
        //Assign victim valid to false
        pageTable.entry[invert_table[victimFrame].pid][invert_table[victimFrame].page].valid = false;
        //Check victim dirty bit to possible write to disk if dirty is true
        if(pageTable.entry[invert_table[victimFrame].pid][invert_table[victimFrame].page].dirty == true)
        {
            //Increment write since it's a dirty bit
            disk_write(pageTable.entry[invert_table[victimFrame].pid][invert_table[victimFrame].page].frameNo, victimPID, victimPID);
            //Assign Dirty to old address
            pageTable.entry[invert_table[victimFrame].pid][invert_table[victimFrame].page].dirty = false;
        }
        
        //***ASSIGN FRAME, VALID, DIRTY TO REPLACEMENT
        //Assign replacement frameNo
        pageTable.entry[pid][pageNo].frameNo = victimFrame;
        //Assign Valid
        pageTable.entry[pid][pageNo].valid = true;
        //Adding the pid & pageNo to invert_table struct
        invert_table[pageTable.entry[pid][pageNo].frameNo].pid = pid;
        invert_table[pageTable.entry[pid][pageNo].frameNo].page = pageNo;
        //Assign Dirty (optional)
        if(type == 'W')
        {
            //Increment Write
            disk_read(pageTable.entry[pid][pageNo].frameNo, pid, pageNo);
            //Set Dirty Bit
            pageTable.entry[pid][pageNo].dirty = true;
        }
        else
        {
            //Increment Reads
            disk_read(pageTable.entry[pid][pageNo].frameNo, pid, pageNo);
            pageTable.entry[pid][pageNo].dirty = false;
	}
        
        return pageTable.entry[pid][pageNo].frameNo;
    }
    else //ELSE: Enters if PT is not full
    {
        lruFlag = 0; //Set lruFlag for not full to 0
        lfuFlag = 0; //Set lfuFlag for not full to 0
        
        //***ASSIGN FRAME, VALID, DIRTY TO ENTRY
        //Assign Valid
        pageTable.entry[pid][pageNo].valid = true;
        //Assign Frame number
        pageTable.entry[pid][pageNo].frameNo = frameIndex;
        
        //LRU IMPLEMENETATION
        if( replacementPolicy == 2 || replacementPolicy == 3 )
        {
            frameReference = pageTable.entry[pid][pageNo].frameNo;
            page_replacement();
        }
        
        //Adding the pid & pageNo to invert_table struct
        invert_table[pageTable.entry[pid][pageNo].frameNo].pid = pid;
        invert_table[pageTable.entry[pid][pageNo].frameNo].page = pageNo;
        //Assign Dirty (optional)
        if(type == 'W')
        {
            //Increment Write
            disk_read(pageTable.entry[pid][pageNo].frameNo, pid, pageNo);
            //Set Dirty Bit
            pageTable.entry[pid][pageNo].dirty = true;
        }
        else
        {
            //Increment Reads
            disk_read(pageTable.entry[pid][pageNo].frameNo, pid, pageNo);
            pageTable.entry[pid][pageNo].dirty = false;
        }
        
        //INCREMENTING FRAME INDEX
        frameIndex = frameIndex + 1;
        
        return pageTable.entry[pid][pageNo].frameNo;
    }
}



/*
 * NAME: memoryManagementUnit | PARAM: int, int, char, bool | RETURN: int
 * PURPOSE: Provided by professor Less
 */
int memoryManagementUnit(int pid, int addr, char type, bool *hit)
{
    int frameNo;
    int pageNo = (addr >> 8);
    int offset = addr - (pageNo << 8);
    
    if(pageNo > MAX_PAGE)
    {
        printf("invalid page number (MAX_PAGE = 0x%x): pid %d, addr %x\n", MAX_PAGE, pid, addr);
        
        return -1;
    }
    
    if(pid > MAX_PID)
    {
        printf("invalid pid (MAX_PID = %d): pid %d, addr %x\n", MAX_PID, pid, addr);
        
        return -1;
    }
    
    //Call to hitTest
    frameNo = hitTest(pid, pageNo, type);
    
    if(frameNo > -1)
    {
        *hit = true;
        
        return (frameNo << 8) + offset;
    }
    
    //Call to pageFaultHandler - missed have to put into physical memory
    frameNo = pageFaultHandler(pid, pageNo, type, hit);
    
    //Returns: the address
    return (frameNo << 8) + offset;
}



/*
 * NAME: printPageTableStats | PARAM: None | RETURN: void
 * PURPOSE: Provided by professor Less
 */
void printPageTableStats()
{
    int req = pageTable.stats.hitCount + pageTable.stats.missCount;
    int hit = pageTable.stats.hitCount;
    int miss = pageTable.stats.missCount;
    
    printf("Request: %d\n", req);
    printf("Hit: %d (%.2f%%)\n", hit, (float) hit * 100 / (float)req);
    printf("Miss: %d (%.2f%%)\n", miss, (float)miss * 100 / (float)req);
}



/*
 * NAME: pageTableInit | PARAM: None | RETURN: void
 * PURPOSE: Provided by professor Less
 */
void pageTableInit()
{
    //---VARIABLE(S)---
    int i;
    int j;
    int lruFlag = 0;
    
    //Initalizing hit & miss count to zero
    pageTable.stats.hitCount = 0;
    pageTable.stats.missCount = 0;
    
    //Initalize the multidimensional array
    for(i = 0; i < MAX_PID; i++)
    {
        for(j = 0; j < MAX_PAGE; j++)
        {
            pageTable.entry[i][j].valid = false;
        }
    }
}