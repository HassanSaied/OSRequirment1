//
// Created by hassan on 10/27/17.
//

#ifndef OSREQUIRMENT_GENERIC_QUEUE_H
#define OSREQUIRMENT_GENERIC_QUEUE_H

#include <stdlib.h>
#include <defs.h>
typedef struct generic_queue_node generic_queue_node;
struct generic_queue_node
{
    void * data;
    generic_queue_node * next;
};
typedef struct generic_queue_head generic_queue_head;
struct generic_queue_head
{
    generic_queue_node * first;
    generic_queue_node * last;
};

generic_queue_head * init_generic_queue();
bool generic_queue_enqueue(generic_queue_head *head, void *new_node);
void * generic_queue_dequeue(generic_queue_head *head);
bool generic_queue_empty(generic_queue_head * head);
#endif //OSREQUIRMENT_GENERIC_QUEUE_H
