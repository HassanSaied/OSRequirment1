#ifndef OSREQUIRMENT_SRTN_QUEUE_H
#define OSREQUIRMENT_SRTN_QUEUE_H

#include <defs.h>
#include <process_data.h>

void srtn_queue_init(void);
bool srtn_queue_empty(void);
void srtn_queue_push(process_data * data);
process_data * srtn_queue_pop(void);
void srtn_queue_destroy(void);

#endif //OSREQUIRMENT_SRTN_QUEUE_H
