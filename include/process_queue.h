#ifndef OSREQUIRMENT_PROCESS_QUEUE_H
#define OSREQUIRMENT_PROCESS_QUEUE_H

#include <generic_queue.h>
#include <process_struct.h>
struct process_queue
{
    generic_queue_head * head;
};
typedef struct process_queue process_queue;
process_queue * init();

bool enqueue(process_queue * queue, process_struct * new_node);
process_struct * dequeue(process_queue * queue);
bool empty(process_queue * queue);
void destroy(process_queue * queue);
#endif //OSREQUIRMENT_PROCESS_QUEUE_H
