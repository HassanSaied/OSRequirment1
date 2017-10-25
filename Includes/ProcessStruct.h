//
// Created by hassan on 10/25/17.
//

#ifndef OSREQUIRMENT_PROCESSSTRUCT_H
#define OSREQUIRMENT_PROCESSSTRUCT_H

struct process {
    int arrivalTime;
    int priority;
    int runningTime;
    int ID;
};
typedef struct process process;

#endif //OSREQUIRMENT_PROCESSSTRUCT_H
