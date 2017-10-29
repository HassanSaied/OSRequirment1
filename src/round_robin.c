#include <round_robin.h>
#include <process_data.h>
#include <process_queue.h>

#include <stdio.h>

static process_queue *circular_queue;

bool enqueue_circular(process_queue * queue, process * new_node)
{
    if(!enqueue(queue, new_node)) return 0;
    queue->head->last->next = queue->head->first;
    return 1;
}

process * dequeue_circular(process_queue * queue)
{
    process *deq_process = dequeue(queue);
    queue->head->last->next = queue->head->first;
    
}

bool empty_circular(process_queue * queue)
{
    return empty(queue);
}

void round_robin()
{
    int getClk();
    int Recmsg(process * pData);

    int x = getClk();
    printf("Scheduler: current time is %d\n", x);
    //////To receive something from the generator, for example  id 2
    process pD;
    printf("Waiting for message...\n");
    while (Recmsg(&pD) == -1); // Wait for message
    process_data data = init_process_data(&pD);
    printf("Round Robin: received process ID: %d\n", data.inner_process.ID);
}
