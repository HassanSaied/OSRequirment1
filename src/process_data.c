#include <process_data.h>
#include <stdio.h>
process_data *init_process_data(process *inner_process){
    process_data *data = (process_data*)malloc(sizeof(process_data));
    data->inner_process = *inner_process;
    data->state = WAITING;
    data->start_time = -1;
    data->remaining_time = inner_process->runningTime;
    data->finish_time = -1;
    return data;
}