#include <round_robin.h>
#include <process_data.h>
#include <stdlib.h>  
#include <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

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

void run_round_robin(int quantum){
    int getClk();

    process_data *process_to_run = dequeue_circular(circular_queue);
    if(process_to_run == NULL) return;
    int time_to_spend = min(quantum, process_to_run->remaining_time);
    pid_t child_pid = fork();
    if(child_pid == -1)
        perror("Round Robin: error in forking process\n");
    else if(child_pid == 0){
        char time_string[64];
        sprintf(time_string, "%d", time_to_spend);
        if (execl(PROCESS_PROCESS_IMAGE_NAME, PROCESS_PROCESS_IMAGE_NAME , time_string , (char *) NULL) == -1) {
            perror("Round Robin: error in running process, terminating this child...\n");
            exit(1);
        }
        printf("Child\n");
    }
    else{
        int curr_time = getClk();
        char state[8];
        if(process_to_run->start_time == -1){
            process_to_run->start_time = curr_time; 
            strcpy(state, "started");
        }else
            strcpy(state, "resumed");
        

        printf("At time %d process %d %s arr %d total %d remain %d wait %d\n",
        curr_time, process_to_run->process.id, state, process_to_run->process.arrivalTime,
        process_to_run->process.runningTime, process_to_run->remaining_time, 
        process_to_run->start_time-process_to_run->process.arrivalTime);
        sleep(time_to_spend);
        curr_time = getClk();
        process_to_run->remaining_time-=time_to_spend;
        if(process_to_run->remaining_time == 0)
            printf("At time %d process %d ended arr %d total %d remain %d wait %d TA %d WTA %.3f\n",
            curr_time, process_to_run->process.id, process_to_run->process.arrivalTime,
            process_to_run->process.runningTime, process_to_run->remaining_time, 
            process_to_run->start_time-process_to_run->process.arrivalTime, 
            curr_time-process_to_run->start_time, 
            (float)(curr_time-process_to_run->start_time)/(float)process_to_run->process.runningTime);
        else
            printf("At time %d process %d stopped arr %d total %d remain %d wait %d\n",
            curr_time, process_to_run->process.id, process_to_run->process.arrivalTime,
            process_to_run->process.runningTime, process_to_run->remaining_time, 
            process_to_run->start_time-process_to_run->process.arrivalTime);
    }

}

void round_robin(int quantum){

    int getClk();
    int Recmsg(process_struct * pData);

    circular_queue = init_circular_queue();
    process_struct pD;
    int msg_status = -1;

    while(msg_status != 1 || !empty_circular(circular_queue)){

        run_round_robin(quantum);

        if ((msg_status = Recmsg(&pD)) == 1)
            break;
        else if(msg_status == 0){
            while(msg_status == 0){
                process_data *data = (process_data *) malloc(sizeof(process_data));
                data = process_data_init(&pD);
                add_process(data);
                msg_status = Recmsg(&pD);
            }
        }
    }
}
