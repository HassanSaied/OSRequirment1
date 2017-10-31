#include <queue_utilities.h>

void initQueue(bool flag)
{
    if (flag)
    {
        qid = msgget(QUEUEKEY, IPC_CREAT  |   0644);
        if (qid == -1)
        {
            perror("queue not created");
        }
    }
    else
    {
        qid = msgget(QUEUEKEY, 0444);
        if (qid == -1)
        {
            //Make sure that the Clock exists
            printf("Error, queue not initialized yet\n");
            kill(getpgrp(), SIGINT);
        }
    }
}


int Sendmsg(process_struct pData)
{
    struct messagebuffer msg;
    msg.data = pData;
    msg.mtype = 1L;
    return msgsnd(qid, &msg, sizeof(msg) - sizeof(long), !IPC_NOWAIT);
}

int Recmsg(process_struct * pData)
{
    struct messagebuffer msg;
    msg.mtype = 1L;
    int ret = msgrcv(qid, &msg, sizeof(msg) - sizeof(long), 0, IPC_NOWAIT);
    *pData = msg.data;
    if (ret == -1)
    {
        return -1;
    }
    if (msg.mtype == ENDTYPE)
    {
        return  1;
    }
    return 0;
}


void lastSend()
{
    struct messagebuffer msg;
    msg.mtype = ENDTYPE;
    msgsnd(qid, &msg, sizeof(msg) - sizeof(long), !IPC_NOWAIT);
}

void destroyQueueAndExit(int x)
{
    msgctl(qid, IPC_RMID, (struct msqid_ds *)0);
    exit(0);
}
