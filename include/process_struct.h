#ifndef OSREQUIRMENT_PROCESS_STRUCT_H
#define OSREQUIRMENT_PROCESS_STRUCT_H

struct process
{
    int arrivalTime;
    int priority;
    int runningTime;
    int ID;
};
typedef struct process process;

#endif //OSREQUIRMENT_PROCESS_STRUCT_H
