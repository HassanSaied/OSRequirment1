#include "clkUtilities.h"
#include "queueUtilities.h"
#include "process_struct.h"
#include <string.h>
struct process_data
{
    process inner_process;

    enum {WAITING , RUNNING } state;
    int start_time; //bd2 emta
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

enum {HPS , SRTN , RR} algorithm_type;

int main(int argc, char* argv[]) {
    initQueue(false);
    initClk();
    //TODO: implement the scheduler :)
    if(strcmp(argv[1], "HPS"))
        algorithm_type = HPS;
    else if(strcmp(argv[1],"SRTN"))
        algorithm_type = SRTN;
    else if(strcmp(argv[1],"RR"))
        algorithm_type = RR;
    else 
    {
        puts("Error 404: Algorithm NOT found!");
        exit(1);
    }
    //Three function, to be called according to algorithm state;
    //===================================
    //Preimplemented Functions examples

    /////Toget time use the following function
    int x= getClk();
    printf("current time is %d\n",x);

    //////To receive something from the generator, for example  id 2
    process pD;
    while(Recmsg(&pD)==-1); //returns -1 on failure; 1 on the end of processes, 0 no processes available yet
    process_data data = init_process_data(&pD);
    return 0;

}
