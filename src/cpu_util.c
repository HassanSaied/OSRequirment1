#include <cpu_util.h>
#include <stdio.h>

#define max(a, b) a > b ? a : b

process *get_next_process(process_queue *queue){
    enqueue(queue, dequeue(queue));
    return (process *)queue->head->first->data;
}

int get_free_time(process_queue *queue){
    process *next_process = (process *)queue->head->first->data;
    process *last_process = (process *)queue->head->last->data; 
    int current_time = 1, free_time = 0;

    while(next_process != last_process){
        free_time += ( max(next_process->arrivalTime - current_time, 0) );
        current_time += ( free_time + next_process->runningTime - 1);
        next_process = get_next_process(queue);
    }
    return free_time;
}

