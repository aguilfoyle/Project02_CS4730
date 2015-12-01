#include <stdio.h>
#include "vm.h"

int numDiskRead = 0;
int numDiskWrite = 0;

//Call to read a page from the disk into the main memory
void disk_read(int frame, int pid, int page)
{
    numDiskRead++;
}

//Write out a dirty page to the disk
void disk_write(int frame, int pid, int page)
{
    numDiskWrite++;
}

void printDiskStats()
{
    printf("Disk read: %d\n", numDiskRead);
    printf("Disk write: %d\n", numDiskWrite);
}