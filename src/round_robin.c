#include <round_robin.h>
#include <process_data.h>
#include <process_struct.h>
#include <logger.h>

#include <stdlib.h>  
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>
#include  <sys/types.h>
#include <logger.h>
#define min(a,b) a<b ? a:b

static struct process_data_queue_struct{
    generic_queue_head * head;
}*circular_queue;
typedef struct process_data_queue_struct process_data_queue;

process_data_queue *init_circular_queue(){
    process_data_queue *queue = (process_data_queue *)malloc(sizeof(process_data_queue));
    queue->head = init_generic_queue();
    return queue;
}

bool empty_circular(process_data_queue *queue){
    return generic_queue_empty(queue->head);
}

bool enqueue_circular(process_data_queue *queue, process_data *new_node){
    bool ret = generic_queue_enqueue(queue->head , (void *)new_node);
    if(ret == false) return false;

    if(queue->head->first != queue->head->last)
        queue->head->last->next = queue->head->first;
    return true;
}

process_data *dequeue_circular(process_data_queue *queue){
    process_data *data = (process_data *)generic_queue_dequeue(queue->head);
    if(data == NULL) return NULL;
    
    if(queue->head->first == queue->head->last && queue->head->first!=NULL)
        queue->head->last->next = NULL;
    return data;
}

void add_process(process_data *data){
    enqueue_circular(circular_queue, data);
}

void run_child(int quantum){
    char time_string[64];
    sprintf(time_string, "%d", quantum);

    if (execl(PROCESS_PROCESS_IMAGE_NAME, PROCESS_PROCESS_IMAGE_NAME , time_string , (char *) NULL) == -1) {
        perror("Round Robin: error in running process, terminating this child...\n");
        exit(1);
    }
}

void run_parent(int quantum, process_data *pro){
    if(pro->start_time == -1){
        pro->start_time = getClk(); 
        pro->state = STARTED;
    }else
        pro->state = RESUMED;
    logger_log(pro);
   
    sleep(quantum);

    pro->remaining_time -= quantum;
    pro->state = (pro->remaining_time>0 ? STOPPED:FINISHED);
    logger_log(pro);
}

void run_round_robin(int quantum){
    process_data *pro = dequeue_circular(circular_queue);
    if(pro == NULL) return;

    pid_t child_pid = fork();
    if(child_pid == -1)
        perror("Round Robin: error in forking process.\n");
    else if(child_pid == 0)
        run_child(quantum);
    else
        run_parent(quantum, pro);

}

void round_robin(int quantum){
    int Recmsg(process_struct *pData);

    circular_queue = init_circular_queue();
    process_struct pD;
    int msg_status;

    do{

        run_round_robin(quantum);

        while((msg_status = Recmsg(&pD))  == 0){
                process_data *data = (process_data *) malloc(sizeof(process_data));
                data = process_data_init(&pD);
                add_process(data);
        }
    }while(msg_status != 1 || !empty_circular(circular_queue));
}