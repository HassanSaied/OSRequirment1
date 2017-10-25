#include <clkUtilities.h>
#include <queueUtilities.h>
#include <defs.h>
#include <ProcessStruct.h>
#include <cstring>
void ClearResources(int);
//TODO : pass our own data structure instead of this array
void readProcessFromFile(int &processCount, process ** &processList);
void testReadProcess(process ** processList , const int & processCount)
{
    for (int i = 0; i < processCount; ++i) {
        printf("%d , %d , %d , %d\n" , processList[i]->ID , processList[i]->arrivalTime,
               processList[i]->runningTime , processList[i]->priority);
    }
}
int main() {

    int processCount;
    process** processList;
    readProcessFromFile(processCount , processList);
    //This is a test function, used purely for testing, remove it in the final product
    testReadProcess(processList , processCount);
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
void readProcessFromFile(int &processCount, process ** &processList)
{
    processList = (process **) malloc(10*sizeof(process*));
    if(processList == NULL)
        return;
    processCount = 0;
    FILE * processFile = fopen(PROCESS_FILE_NAME ,"r");
    char LineIdentifier[10],ignoredEndLine;
    while(fscanf(processFile , "%s" ,LineIdentifier)!= EOF)
    {
        if(LineIdentifier[0] != '#')
        {
            //allocate a memory for new process
            process * newProcess = (process *) malloc(sizeof(process));
            if(newProcess == NULL) //TODO: really must change this behaviour, 7mdy may kill me for this
                continue;
            newProcess->ID = atoi(LineIdentifier);
            fscanf(processFile , "%d%d%d%c" ,&newProcess->arrivalTime,&newProcess->runningTime
                    ,&newProcess->priority,&ignoredEndLine);
            processList[processCount++] = newProcess;
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




