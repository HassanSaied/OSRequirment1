#include <clk_utilities.h>
#include <queue_utilities.h>
#include <defs.h>
#include <algorithms.h>
#include <process_struct.h>
#include <process_queue.h>
#include <hpf_red_black_tree.h>
#include <srtn_red_black_tree.h>
#include <process_data.h>
#include <stdio.h>
#include <stdlib.h>

void clear_resources(int);
void read_process_from_file(process_queue * queue);
int create_clock(void);
int create_scheduler(char * const * argv);

void testReadProcess(process_queue * queue)
{
    while (!empty(queue))
    {
        process * currentProcess = dequeue(queue);
        printf("%d , %d , %d , %d\n" , currentProcess->ID , currentProcess->arrivalTime,
               currentProcess->runningTime , currentProcess->priority);
    }
}
void print_queue(process_queue * queue)
{
    generic_queue_node * ptr = queue->head->first;
    do{
        process * current_process = (process*) ptr->data;
        printf("%d , %d , %d , %d\n" , current_process->ID, current_process->arrivalTime,
               current_process->runningTime , current_process->priority);
        ptr = ptr->next;

    }while (ptr !=NULL);
}
void test_priority_r_b_tree(process_queue * queue ,rb_red_blk_tree * tree)
{
    //This function does an absolutely wrong cast, but it freakin' works, please use it for test only
    while(!empty(queue))
    {
        process * current_process = dequeue(queue);
        hpf_tree_insert(tree, init_process_data(current_process));
    }
    while(tree->root->left != tree->nil)
    {
        rb_red_blk_node * current_node = hpf_get_next_process(tree);
        enqueue(queue,&(hpf_tree_delete(tree, current_node))->inner_process);
    }
    print_queue(queue);
}
void test_time_rb_tree(process_queue * queue , rb_red_blk_tree * tree)
{
    while(!empty(queue))
    {
        process * current_process = dequeue(queue);
        process_data* current_process_data = init_process_data(current_process);
        srtn_tree_insert(tree, current_process_data);
    }
    while(tree->root->left != tree->nil)
    {
        rb_red_blk_node * current_node = srtn_get_next_process(tree);
        enqueue(queue,&(srtn_tree_delete(tree, current_node))->inner_process);
    }
    print_queue(queue);
}
int main()
{
    process_queue * queue = init();
    read_process_from_file(queue);
    /*
    test_priority_r_b_tree(queue , hpf_init_tree());
    puts("=============================================");
    test_time_rb_tree(queue , srtn_init_tree());
    //This is a test function, used purely for testing, remove it in the final product
    */
    initQueue(true);

    printf("Please choose one of the following algorithms to use for scheduling:\n");
    printf("%d-%s\n%d-%s\n%d-%s\n", HPF, ALGORITHM_TYPE_STRINGS[HPF], 
    SRTN, ALGORITHM_TYPE_STRINGS[SRTN], RR, ALGORITHM_TYPE_STRINGS[RR]);
    int algorithm_choice, quantum;
    while((algorithm_choice = getchar()-'0')!= HPF && algorithm_choice!= SRTN 
            && algorithm_choice!= RR){
        printf("Please enter a vaild choice!\n");
        printf("Please choose one of the following algorithms to use for scheduling:\n");
        printf("%d-%s\n%d-%s\n%d-%s\n", HPF, ALGORITHM_TYPE_STRINGS[HPF], 
        SRTN, ALGORITHM_TYPE_STRINGS[SRTN], RR, ALGORITHM_TYPE_STRINGS[RR]);
        getchar(); // Get rid of enter
    }
    if(algorithm_choice == RR){
        printf("Please enter a value to be used as quantum: ");
        while(scanf("%d", &quantum) != 1 && quantum <= 0){
            printf("Please enter a vaild value!\n");  
        }
    }
    create_clock();
    char quantum_string[64];
    sprintf(quantum_string, "%d", quantum);
    char * argv[] = {SCHEDULER_PROCESS_IMAGE_NAME ,"" , quantum_string , NULL};
    argv[1] = ALGORITHM_TYPE_STRINGS[algorithm_choice];
    int scheduler_pid = create_scheduler(argv);
    //will be removed
   
    sleep(2);
    // 3-use this function AFTER creating clock process to initialize clock, and initialize MsgQueue
    initClk();
    //TODO:  Generation Main Loop
    //4-Creating a data structure for process  and  provide it with its parameters
    //5-Send & Notify the information to  the scheduler at the appropriate time
    //(only when a process arrives) so that it will be put it in its turn.
    //===================================
    //Preimplemented Functions examples
    /////Toget time use the following function
    int current_second;
    process* pD = NULL;
    while(!empty(queue) || pD != NULL){
        current_second = getClk();
        if(pD == NULL)
            pD = dequeue(queue);
        if(pD->arrivalTime == current_second){
            Sendmsg(*pD); //returns -1 on failure;
            pD = NULL;
        }
    }
    //no more processes, send end of transmission message
    lastSend();
    //////////To clear all resources
    int child_status;
    waitpid(scheduler_pid, &child_status, 0);
    clear_resources(0);
    //======================================
}

void clear_resources(int x){
    msgctl(qid, IPC_RMID, (struct msqid_ds *)0);
    destroyClk(true);
    exit(0);
}

void read_process_from_file(process_queue * queue){
    FILE * processFile = fopen(PROCESS_FILE_NAME , "r");
    char lineIdentifier[10], ignoredEndLine;
    while (fscanf(processFile , "%s" , lineIdentifier) != EOF)
    {
        if (lineIdentifier[0] != '#')
        {
            //allocate a memory for new process
            process * new_process = (process *) malloc(sizeof(process));
            if (new_process == NULL) //TODO: really must change this behaviour, 7mdy may kill me for this
            {
                continue;
            }
            new_process->ID = atoi(lineIdentifier);
            fscanf(processFile , "%d%d%d%c" , &new_process->arrivalTime, &new_process->runningTime
                   , &new_process->priority, &ignoredEndLine);
            enqueue(queue , new_process);
        }
        else
        {
            char ignoredBuffer [80];
            fgets(ignoredBuffer , sizeof(ignoredBuffer) , processFile);
        }
    }
    fclose(processFile);
    return;
}

int create_clock(void){
    int clock_pid;
    if ((clock_pid = fork()) == 0)
    {
        if (execl(CLOCK_PROCESS_IMAGE_NAME , CLOCK_PROCESS_IMAGE_NAME , (char *) NULL) == -1)
        {
            puts("Error while creating the clock, terminating this child...");
            exit(1);
        }
    }
    else
    {
        return clock_pid;
    }
}

int create_scheduler(char * const * argv){
    int scheduler_pid;
    if ((scheduler_pid = fork()) == 0)
    {
        //This is the child
        if (execv(SCHEDULER_PROCESS_IMAGE_NAME , argv) == -1)
        {
            puts("Error while creating the scheduler, terminating this child...");
            exit(1);
        }
    }
    else
    {
        return scheduler_pid;
    }
}
