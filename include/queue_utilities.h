#ifndef OSREQUIRMENT_QUEUE_UTILS
#define OSREQUIRMENT_QUEUE_UTILS

#include <stdio.h>      //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <process_struct.h>
#include "defs.h"

#define QUEUEKEY 777
#define ENDTYPE 19L

///==============================
//don't mess with this variable//
int qid;
//===============================

struct messagebuffer
{
    long mtype;
    process data;
};


void initQueue(bool flag);


int Sendmsg(process pData);

int Recmsg(process * pData);


void lastSend();

void destroyQueueAndExit(int x);

#endif