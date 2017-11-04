#include <round_robin.h>
#include <process_data.h>
#include <process_struct.h>
#include <logger.h>
#include <clk_utilities.h>
#include <queue_utilities.h>

#include <stdlib.h>  
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#define min(a,b) a<b ? a:b

static int rr_quant;
static process_data *curr_pro;
static pid_t timer_pid; 

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

pid_t set_timer(void){
    pid_t clock_pid;
    if ((clock_pid = fork()) == -1){
        perror("Error while forking the clock");
        exit(1);
    }
    else if (!clock_pid){
        char time_string[64];
        sprintf(time_string, "%d", rr_quant);
        if (execl(PROCESS_PROCESS_IMAGE_NAME , PROCESS_PROCESS_IMAGE_NAME, time_string, "Timer", (char *) NULL) == -1){
            perror("Error while creating the clock");
            exit(1);
        }
        exit(1);
    }else{
        return clock_pid;
    }
}

void rr_wake_up(){
    printf("RR: Waking up.\n");
    if(curr_pro->state != FINISHED){
        if((curr_pro->remaining_time -= rr_quant) > 0){
            kill(curr_pro->pid, SIGSTOP);
            curr_pro->state = STOPPED;
            printf("@T=%d RR: stopped %d\n", getClk(), curr_pro->process.id);
            logger_log(curr_pro);
            enqueue_circular(circular_queue, curr_pro);
        }else if(!curr_pro->remaining_time){
            int status;
            waitpid(curr_pro->pid, &status, 0);
            curr_pro->state = FINISHED;
            curr_pro->finish_time = getClk();
            curr_pro->remaining_time = 0;
            printf("@T=%d RR: Child termination signal received.\n", getClk());
            printf("@T=%d RR: finished %d\n", getClk(), curr_pro->process.id);
            logger_log(curr_pro);
            free(curr_pro);
        }else
            perror("RR: child signal termination signal sent after quantum finished\n");
    }
}

void rr_sigchild_handler(int signo){
    int status;
    pid_t pid;
    printf("@T=%d RR: Signal handler provoked\n", getClk());
    if((pid = waitpid(-1, &status, WNOHANG))==curr_pro->pid){
        printf("@T=%d RR: Signal handler provoked by process\n", getClk());
        if(WIFEXITED(status)){
            curr_pro->state = FINISHED;
            curr_pro->finish_time = getClk();
            curr_pro->remaining_time = 0;
            printf("@T=%d RR: Child termination signal received.\n", getClk());
            printf("@T=%d RR: finished %d\n", getClk(), curr_pro->process.id);
            logger_log(curr_pro);
            kill(timer_pid, SIGKILL);
            timer_pid=-1;
            free(curr_pro);
        }else{
            puts("Child Not yet Terminated");
            pause();
        }
    }else if(pid==timer_pid){
        printf("@T=%d RR: Signal handler provoked by timer\n", getClk());
        if(WIFEXITED(status)){
            rr_wake_up();
            timer_pid=-1;
        }
        else 
            pause();
    }
}

void rr_start_process(process_data *pro){
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
        printf("@T=%d RR: started %d\n", getClk(), curr_pro->process.id);
        logger_log(curr_pro);

        timer_pid=set_timer();
        while(curr_pro->state!=FINISHED&&timer_pid!=-1)
            pause();
    }
}

void rr_resume_process(process_data *pro){
    pro->state = RESUMED;
    curr_pro = pro;
    kill(curr_pro->pid, SIGCONT);
    printf("@T=%d RR: resumed %d\n", getClk(), curr_pro->process.id);
    logger_log(curr_pro);

    timer_pid=set_timer();
    while(curr_pro->state!=FINISHED&&timer_pid!=-1)
        pause();
}

void rr_run_next_process(){
    process_data *pro;
    if((pro = dequeue_circular(circular_queue)) == NULL) return;
    
    if(pro->start_time == -1) {
        rr_start_process(pro);
    }
    else
        rr_resume_process(pro);
}

void round_robin(int quantum){
    rr_quant = quantum;
    circular_queue = init_circular_queue();
    process_struct pD;
    int msg_status;
    int flag = 0;
    if(signal(SIGCHLD, rr_sigchild_handler) == SIG_ERR){
        printf("RR: can't register signal handler\n");
        exit(1);
    }

    do{
        rr_run_next_process();

        while((msg_status = Recmsg(&pD))  == 0){
                process_data *data = (process_data *)malloc(sizeof(process_data));
                data = process_data_init(&pD);
                printf("@T=%d RR: received %d\n", getClk(), data->process.id);
                rr_add_process(data);
        }
        if(msg_status == 1) flag = 1;
    }while(!flag || !empty_circular(circular_queue));
    free(circular_queue);
}