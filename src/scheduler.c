#include <clk_utilities.h>
#include <highest_priority_first.h>
#include <srtn_algorithm.h>
#include <round_robin.h>
#include <process_data.h>
#include <queue_utilities.h>
#include <string.h>

int main(int argc, char * argv[])
{
    initQueue(false);
    initClk();
    signal(SIGUSR1, SIG_IGN);
    if (!strcmp(argv[1], "HPF"))
    {
        printf("Scheduler: Highest Priority First detected!\n");
        highest_priority_first();
    }
    else if (!strcmp(argv[1], "SRTN"))
    {
        printf("Scheduler: Shortest Remaining Time Next detected!\n");
        shortest_remaining_time_next();
    }
    else if (!strcmp(argv[1], "RR"))
    {
        printf("Scheduler: Robin Round detected with quantum = %s.\n", argv[2]);
        round_robin(atoi(argv[2]));
    }
    else
    {
        printf("Error 404: Algorithm NOT found!\n");
        exit(1);
    }
    printf("Scheduler: done!\n");
}
