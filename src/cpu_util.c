#include <cpu_util.h>
#include <stdio.h>

#define max(a, b) a > b ? a : b

int get_free_time(process_queue *queue){
    generic_queue_node *next_node = (generic_queue_node *)queue->head->first;
    int current_time = 1, free_time = 0;

    while(next_node != NULL){
        free_time += ( max(((process *)next_node->data)->arrivalTime - current_time, 0) );
        current_time += ( free_time + ((process *)next_node->data)->runningTime - 1);
        next_node = next_node->next;
    }
    return free_time;
}

