#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "vm.h"

/*
 * @function main
 * @brief main function used to generate the request.
 * @param argc - number of arguments.
 * @return 0 upon completion and success
 */
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("usage: ./input_gen number_of_request\n");
        return -1;
    }
    
    //*** VARIABLE(S) ***
    //Integer(s)
    int req_num = atoi(argv[1]);//Request Numbers
    int i;
    int pid;
    int page;
    int offset;
    int addr;
    //Character(s)
    char type;
    
    srand(time(NULL)); //to generate unique random numbers each time ran
    
    //Main for loop to generate the correct number of request
    for(i = 0; i < req_num; i++)
    {
        pid = rand() % MAX_PID;
        page = rand() % MAX_PAGE;
        offset = rand() % 0xFF;
        
        if(rand() % 2 == 0)
        {
            type = 'R';
        }
        else
        {
            type = 'W';
        }
        
        //Creates the hex value for the address
        addr = (page << 8) + offset;
        
        printf("%d %c 0x%x\n", pid, type, addr);
    }
    
    return 0;
}