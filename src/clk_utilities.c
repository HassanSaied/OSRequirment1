//
// Created by hassan on 10/31/17.
//
#include <clk_utilities.h>
int getClk()
{
    int clk = *shmaddr;
    return clk;
}


/* All process call this function at the begining to establish communication
between them and the clock module

Again, Remember that the clock is only emulation
*/
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        //Make sure that the Clock exists
        printf("wait, Clock not initialized yet\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *) shmat(shmid, (void *)0, 0);
}



/* All process call this function at the end to release the  communication
resources between them and the clock module

Again, Remember that the clock is only emulation

input: terminateAll : is a flag to indicate whether that
this is the end of simulation it terminates all the system and release resources
*/

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}