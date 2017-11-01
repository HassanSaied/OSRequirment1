//
// Created by hassan on 11/1/17.
//

#ifndef OSREQUIRMENT_INTEGER_QUEUE_H
#define OSREQUIRMENT_INTEGER_QUEUE_H

#include <generic_queue.h>
typedef struct integer_queue
{
    generic_queue_head * head;
} integer_queue;

integer_queue * integer_queue_init();
bool integer_queue_enqueue(integer_queue * queue, int  new_node);
int  integer_queue_dequeue(integer_queue * queue);
bool integer_queue_empty(integer_queue * queue);
void integer_queue_destroy(integer_queue *queue);
#endif //OSREQUIRMENT_INTEGER_QUEUE_H
