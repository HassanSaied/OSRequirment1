#ifndef OSREQUIRMENT_PROCESS_DATA_H
#define OSREQUIRMENT_PROCESS_DATA_H

#include <process_struct.h>
#include <stdlib.h>
struct process_data
{
    process inner_process;
    enum {WAITING , RUNNING,FINISHED} state;
    int start_time;
    int remaining_time;
    int finish_time;

};
typedef struct process_data process_data;

process_data * init_process_data(process *inner_process);

#endif //OSREQUIRMENT_PROCESS_DATA_H