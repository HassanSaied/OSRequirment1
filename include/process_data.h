#ifndef OSREQUIRMENT_PROCESS_DATA_H
#define OSREQUIRMENT_PROCESS_DATA_H

#include <process_struct.h>

struct process_data
{
    process inner_process;
    enum {WAITING , RUNNING } state;
    int start_time;
    int remaining_time;

};
typedef struct process_data process_data;

process_data init_process_data(process * inner_process);

#endif //OSREQUIRMENT_PROCESS_DATA_H