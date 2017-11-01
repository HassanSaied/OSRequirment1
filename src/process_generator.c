#include <clk_utilities.h>
#include <queue_utilities.h>
#include <defs.h>
#include <process_struct.h>
#include <process_queue.h>
#include <cpu_util.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_resources(int);
void read_process_from_file(process_queue * queue);
int create_clock(void);
int create_scheduler(char * const * argv);

int main()
{
    process_queue * queue = init();
    read_process_from_file(queue);
    int free_time = get_free_time(queue);
    initQueue(true);

    char algorithm[5];
    int quantum;
    printf("Choose the scheduling algorithm: ");
    scanf("%s", algorithm);
    if (strcmp(algorithm, "HPF") && strcmp(algorithm, "SRTN") && strcmp(algorithm, "RR"))
    {
        printf("Invalid input!\nUsage: process_generator {HPF, SRTN, RR quantum}");
        exit(1);
    }
    if (!strcmp(algorithm, "RR"))
        scanf("%i", &quantum);

    char quantum_string[64], free_time_string[12];
    sprintf(quantum_string, "%d", quantum);
    sprintf(free_time_string, "%d", free_time);
    char * argv[] = {SCHEDULER_PROCESS_IMAGE_NAME, algorithm, quantum_string, free_time_string , NULL};
    int scheduler_pid = create_scheduler(argv);
    create_clock();
    initClk();

    process_struct * pD = dequeue(queue);
    while(pD != NULL)
    {
        int clk = getClk();
        while (pD->arrivalTime != clk)
            clk = getClk();
        do
        {
            if (Sendmsg(*pD) == -1)
            {
                perror("Error sending message to the scheduler");
                abort();
            }
            free(pD);
            pD = dequeue(queue);
        } while (pD != NULL && pD->arrivalTime == clk);
        kill(scheduler_pid, SIGUSR1);
    }

    // No more processes, send end of transmission message
    lastSend();

    // Clear all resources
    int scheduler_status;
    waitpid(scheduler_pid, &scheduler_status, 0);
    destroy(queue);
    clear_resources(0);
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
            process_struct * new_process = (process_struct *) malloc(sizeof(process_struct));
            if (new_process == NULL) //TODO: really must change this behaviour, 7mdy may kill me for this
            {
                continue;
            }
            new_process->id = atoi(lineIdentifier);
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
            perror("Error while creating the clock");
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
            perror("Error while creating the scheduler");
            exit(1);
        }
    }
    else
    {
        return scheduler_pid;
    }
}
