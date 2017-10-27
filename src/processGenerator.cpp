#include <clkUtilities.h>
#include <queueUtilities.h>
#include <defs.h>
#include <process_struct.h>
#include <process_queue.h>
void ClearResources(int);
//TODO : pass our own data structure instead of this array
void readProcessFromFile(process_queue *queue);
void testReadProcess(process_queue *queue)
{
    while(!empty(queue)) {
        process * currentProcess = dequeue(queue);
        printf("%d , %d , %d , %d\n" , currentProcess->ID , currentProcess->arrivalTime,
               currentProcess->runningTime , currentProcess->priority);
    }
}
int main() {
    process_queue * queue = init();
    readProcessFromFile(queue);
    //This is a test function, used purely for testing, remove it in the final product
    testReadProcess(queue);
    initQueue(true);
    //TODO: 
    // 1-Ask the user about the chosen scheduling Algorithm and its parameters if exists.
    // 2-Initiate and create Scheduler and Clock processes.


    // 3-use this function AFTER creating clock process to initialize clock, and initialize MsgQueue
    initClk();

    
    //TODO:  Generation Main Loop
    //4-Creating a data structure for process  and  provide it with its parameters 
    //5-Send & Notify the information to  the scheduler at the appropriate time 
    //(only when a process arrives) so that it will be put it in its turn.

    //===================================
    //Preimplemented Functions examples

    /////Toget time use the following function
    int x= getClk();
    printf("current time is %d\n",x);
    //////Tosend something to the scheduler, for example send id 2
    struct processData pD;
    pD.id = 2;
    Sendmsg(pD); //returns -1 on failure;
    //no more processes, send end of transmission message
    lastSend();
    //////////To clear all resources
    ClearResources(0);
    //======================================
    
}

void ClearResources(int)
{
    msgctl(qid, IPC_RMID, (struct msqid_ds*)0);
    destroyClk(true); 
    exit(0);
}
void readProcessFromFile(process_queue *queue)
{
    FILE * processFile = fopen(PROCESS_FILE_NAME ,"r");
    char lineIdentifier[10],ignoredEndLine;
    while(fscanf(processFile , "%s" ,lineIdentifier)!= EOF)
    {
        if(lineIdentifier[0] != '#')
        {
            //allocate a memory for new process
            process * new_process = (process *) malloc(sizeof(process));
            if(new_process == NULL) //TODO: really must change this behaviour, 7mdy may kill me for this
                continue;
            new_process->ID = atoi(lineIdentifier);
            fscanf(processFile , "%d%d%d%c" ,&new_process->arrivalTime,&new_process->runningTime
                    ,&new_process->priority,&ignoredEndLine);
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




