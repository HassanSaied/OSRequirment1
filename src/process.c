#include <headers.h>
#include <stdlib.h>  
#include <stdio.h>  

/* Modify this file as needed*/
int remaining_time = 0;


int main(int agrc, char * argv[])
{
    //if you need to use the emulated clock uncomment the following line
    //initClk();
    //TODO: it needs to get the remaining time from somewhere
    //remainingtime = ??;
    remaining_time = atoi(argv[1]);
    while (remaining_time > 0)
    {
        sleep(1);
        --remaining_time;
    }
    //if you need to use the emulated clock uncomment the following line
    //destroyClk(false);
    return 0;
}
