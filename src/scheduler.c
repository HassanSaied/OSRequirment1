#include <clk_utilities.h>
#include <queue_utilities.h>
#include <highest_priority_first.h>
#include <shortest_remaining_time_first.h>
#include <round_robin.h>
#include <scheduler.h>

#include <string.h>

int main(int argc, char * argv[])
{
    initQueue(false);
    initClk();
    if (strcmp(argv[1], "HPS"))
    {
        highest_priority_first();
    }
    else if (strcmp(argv[1], "SRTN"))
    {
        shortest_remaining_time_first();
    }
    else if (strcmp(argv[1], "RR"))
    {
        round_robin();
    }
    else
    {
        puts("Error 404: Algorithm NOT found!");
        exit(1);
    }
    //Three function, to be called according to algorithm state;
    //===================================
    //Preimplemented Functions examples
    /////Toget time use the following function
    int x = getClk();
    printf("Scheduler: current time is %d\n", x);
    //////To receive something from the generator, for example  id 2
    process pD;
    printf("Waiting for message...\n");
    while (Recmsg(&pD) == -1); // Wait for message
    process_data data = init_process_data(&pD);
    printf("Scheduler: received process ID: %d\n", data.inner_process.ID);
    return 0;
}
