//
// Created by hassan on 10/27/17.
//

#ifndef OSREQUIRMENT_PROCESS_QUEUE_H
#define OSREQUIRMENT_PROCESS_QUEUE_H

#include <generic_queue.h>
#include <process_struct.h>
struct process_queue
{
    generic_queue_head * head;
};
process_queue* init();

bool enqueue(process_queue *queue, process *new_node);
process * dequeue(process_queue * queue);
bool empty(process_queue * queue);

#endif //OSREQUIRMENT_PROCESS_QUEUE_H
