#include <process_data.h>
#include <stdio.h>
process_data * process_data_init(process_struct * process)
{
    process_data *data = (process_data*)malloc(sizeof(process_data));
    data->process = *process;
    data->pid = -1;
    data->state = STOPPED;
    data->start_time = -1;
    data->remaining_time = process->runningTime;
    data->finish_time = -1;
    return data;
}
void process_data_destroy(process_data * process)
{
    free(process);
}
