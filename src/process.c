#include <headers.h>
#include <stdlib.h>
#include <stdio.h>
#include <clk_utilities.h>
/* Modify this file as needed*/
long int remainingtime = 0;


int main(int agrc, char * argv[])
{
    //if you need to use the emulated clock uncomment the following line
    initClk();
    //TODO: it needs to get the remaining time from
    char * end_ptr;
    remainingtime = strtol(argv[1],&end_ptr , 10);
    //printf("I am the process, my remaining Time is %ld\n",remainingtime);
    long int target_time = remainingtime+getClk();
    while (target_time > getClk());
    //if you need to use the emulated clock uncomment the following line
    //destroyClk(false);
    return 0;
}
