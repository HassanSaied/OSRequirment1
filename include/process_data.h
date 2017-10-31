#ifndef OSREQUIRMENT_PROCESS_DATA_H
#define OSREQUIRMENT_PROCESS_DATA_H

#include <process_struct.h>
#include <stdlib.h>
typedef struct process_data
{
    process_struct process;
    pid_t pid;
    enum {STOPPED, STARTED, RESUMED, FINISHED} state;
    int start_time;
    int remaining_time;
    int finish_time;
} process_data;

process_data * process_data_init(process_struct * process);

#endif //OSREQUIRMENT_PROCESS_DATA_H
