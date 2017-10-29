#include <clk_utilities.h>
#include <queue_utilities.h>
#include <highest_priority_first.h>
#include <shortest_remaining_time_first.h>
#include <round_robin.h>
#include <process_data.h>

#include <string.h>

int main(int argc, char * argv[])
{
    initQueue(false);
    initClk();
    printf("Process generator: entered with algorithm type: %s.\n", argv[1]);
    if (!strcmp(argv[1], "HPF"))
    {
        printf("Scheduler: Highiest Priority First  detected!\n");
        highest_priority_first();
    }
    else if (!strcmp(argv[1], "SRTN"))
    {
        printf("Scheduler: Shortest Remaining Time Next detected!\n");
        shortest_remaining_time_first();
    }
    else if (!strcmp(argv[1], "RR"))
    {
        printf("Scheduler: Robin Round detected with quantum = %s!\n", argv[2]);
        round_robin();
    }
    else
    {
        printf("Error 404: Algorithm NOT found!\n");
        exit(1);
    }
}
