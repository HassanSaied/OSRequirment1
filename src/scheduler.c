#include <clk_utilities.h>
#include <queue_utilities.h>
#include <process_struct.h>
#include <highest_priority_first.h>
#include <shortest_remaining_time_first.h>
#include <round_robin.h>

#include <string.h>

struct process_data
{
    process inner_process;
    enum {WAITING , RUNNING } state;
    int start_time;
    int remaining_time;

};
typedef struct process_data process_data;

process_data init_process_data(process * inner_process)
{
    process_data data;
    data.inner_process = *inner_process;
    data.state = WAITING;
    data.start_time = -1;
    data.remaining_time = inner_process->runningTime;
    return data;
}

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
