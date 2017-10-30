#include <round_robin.h>
#include <process_data.h>
#include <process_queue.h>

#include <stdio.h>

static struct process_data_queue_struct{
    generic_queue_head * head;
}*circular_queue;
typedef struct process_data_queue_struct process_data_queue;

process_data_queue *init_circular_queue(){
    process_data_queue *queue = (process_data_queue *)malloc(sizeof(process_data_queue));
    queue->head = init_generic_queue();
    return queue;
}

bool enqueue_circular(process_data_queue *queue, process_data *new_node){
    bool ret = generic_queue_enqueue(queue->head , (void *)new_node);
    if(ret == false) return false;
    queue->head->last = queue->head->first;
    return true;
}

process_data *dequeue_circular(process_data_queue *queue){
    process_data *data = (process_data *)generic_queue_dequeue(queue->head);
    queue->head->last = queue->head->first;
    return data;
}

bool empty_circular(process_data_queue *queue){
    return generic_queue_empty(queue->head);
}

static void sig_handler(int sig){
}

void add_process(process_data data){
    enqueue_circular(circular_queue, &data);
    printf("Round Robin: prcess add with id: %d\n", data.inner_process.ID);
}

void run_round_robin(int quantum){
    process_data *process_to_run = dequeue_circular(circular_queue);
    if(process_to_run == NULL) return;
}

void round_robin(int quantum){

    int getClk();
    int Recmsg(process * pData);

    circular_queue = init_circular_queue();
    process pD;
    process_data data;
    int msg_status = -1;

    while(msg_status != 1){

        run_round_robin(quantum);

        if ((msg_status = Recmsg(&pD)) == 1)
            return;
        else if(msg_status == 0){
            while(msg_status == 0){
                data = *init_process_data(&pD);
                printf("Round Robin: current time is: %d.", getClk());
                add_process(data);
                msg_status = Recmsg(&pD);
            }
        }
    }
}
