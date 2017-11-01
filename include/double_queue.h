//
// Created by hassan on 11/1/17.
//


#ifndef OSREQUIRMENT_DOUBLE_QUEUE_H
#define OSREQUIRMENT_DOUBLE_QUEUE_H
#include <generic_queue.h>
typedef struct
{
    generic_queue_head * head;
} double_queue;

double_queue* double_queue_init();
bool double_queue_enqueue(double_queue *queue, double new_node);
double double_queue_dequeue(double_queue *queue);
bool double_queue_empty(double_queue *queue);
void double_queue_destroy(double_queue *queue);
#endif //OSREQUIRMENT_INTEGER_QUEUE_H
