#include <headers.h>
#include <stdlib.h>
#include <stdio.h>
#include <clk_utilities.h>

int main(int agrc, char * argv[])
{
    initClk();
    char * end_ptr;
    long int remaining_time = strtol(argv[1], &end_ptr, 10);
    printf("I am the process, my remaining Time is %ld\n",remaining_time);
    while (remaining_time > 0)
    {
        remaining_time--;
        sleep(1);
    }
    destroyClk(false);
    return 0;
}
