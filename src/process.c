#include <headers.h>
#include <stdlib.h>
#include <stdio.h>
#include <clk_utilities.h>

int main(int argc, char * argv[])
{
    initClk();
    char *end_ptr;
    long int remaining_time = strtol(argv[1], &end_ptr, 10);
    if(argc==3){
        char *type = argv[2];
        printf("I am the %s, my remaining Time is %ld\n",type,remaining_time);
    }else
        printf("I am the process, my remaining Time is %ld\n",remaining_time);
    while (remaining_time > 0){
        remaining_time--;
        sleep(1);
    }
    destroyClk(false);
    return 0;
}
