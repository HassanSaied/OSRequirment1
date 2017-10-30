#include <round_robin.h>
#include <process_data.h>
#include <process_queue.h>

#include <stdio.h>

static process_queue *circular_queue;

bool enqueue_circular(process_queue * queue, process * new_node){
    if(!enqueue(queue, new_node)) return 0;
    queue->head->last->next = queue->head->first;
    return 1;
}

process * dequeue_circular(process_queue * queue){
    process *deq_process = dequeue(queue);
    queue->head->last->next = queue->head->first;
    
}

bool empty_circular(process_queue * queue){
    return empty(queue);
}

static volatile int punt;
static void sig_handler(int sig){
    punt = 1;
}

void create_process(process_data data, int timeout){
    printf("Round Robin: %d\n",data.inner_process.arrivalTime);
}

void round_robin(int quantum){

    int getClk();
    int Recmsg(process * pData);

    process pD;
    int msg_status;

    while((msg_status = Recmsg(&pD)) != 1){

        while ( msg_status == -1)
            msg_status = Recmsg(&pD);

        process_data data = init_process_data(&pD);
        printf("Round Robin: current time is: %d.", getClk());
        create_process(data, quantum);
    }
    
}
