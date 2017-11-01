#include <round_robin.h>
#include <process_data.h>
#include <process_struct.h>
#include <logger.h>

#include <stdlib.h>  
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <sys/wait.h>
#include  <sys/types.h>

#define min(a,b) a<b ? a:b

static int rr_quant;
static process_data *curr_pro;

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

void rr_add_process(process_data *data){
    enqueue_circular(circular_queue, data);
}

void rr_wake_up(int sleep_stat){
    int getClk();

    if(!sleep_stat){
        kill(curr_pro->pid, SIGSTOP);
        if((curr_pro->remaining_time -= rr_quant) != 0){
            curr_pro->state = STOPPED;
            logger_log(curr_pro);
            enqueue_circular(circular_queue, curr_pro);
        }else{
            curr_pro->state = FINISHED;
            curr_pro->finish_time = getClk();
            logger_log(curr_pro);
            free(curr_pro);
        }
    }
}

void rr_sigchild_handler(){
    int getClk();

    curr_pro->state = FINISHED;
    curr_pro->finish_time = getClk();
    curr_pro->remaining_time = 0;
    logger_log(curr_pro);
    free(curr_pro);
}

void rr_start_process(process_data *pro){
    int getClk();

    pid_t child_pid = fork();
    if(child_pid == -1)
        perror("Round Robin: error in forking process.\n");
    else if(child_pid == 0){
        char time_string[64];
        sprintf(time_string, "%d", pro->process.runningTime);
    
        if (execl(PROCESS_PROCESS_IMAGE_NAME, PROCESS_PROCESS_IMAGE_NAME , time_string , (char *) NULL) == -1) {
            perror("Round Robin: error in running process, terminating this child...\n");
            exit(1);
        }
    }
    else{
        pro->state = STARTED;
        pro->start_time = getClk(); 
        pro->pid = child_pid;
        curr_pro = pro;
        logger_log(curr_pro);
        rr_wake_up(sleep(rr_quant));
    }
}

void rr_resume_process(process_data *pro){
    int getClk();

    pro->state = RESUMED;
    curr_pro = pro;
    kill(curr_pro->pid, SIGCONT);
    logger_log(curr_pro);   
    rr_wake_up(sleep(rr_quant));
}

void rr_run_next_process(){
    process_data *pro;
    if((pro = dequeue_circular(circular_queue)) == NULL) return;
    
    if(pro->start_time == -1)
        rr_start_process(pro);
    else
        rr_resume_process(pro);
}

void round_robin(int quantum){
    int Recmsg(process_struct *pData);

    rr_quant = quantum;
    circular_queue = init_circular_queue();
    process_struct pD;
    int msg_status;
    int flag = 0;
    signal(SIGCHLD, rr_sigchild_handler);

    do{
        rr_run_next_process();

        while((msg_status = Recmsg(&pD))  == 0){
                process_data *data = (process_data *)malloc(sizeof(process_data));
                data = process_data_init(&pD);
                rr_add_process(data);
        }
        if(msg_status == 1) flag = 1;
    }while(flag != 1 || !empty_circular(circular_queue));
    free(circular_queue);
}