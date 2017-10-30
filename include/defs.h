#ifndef OSREQUIRMENT_DEFS_H
#define OSREQUIRMENT_DEFS_H

#define PROCESS_FILE_NAME "processes.txt"
//This Name must be put in the make file also
#define CLOCK_PROCESS_IMAGE_NAME "clock"
#define SCHEDULER_PROCESS_IMAGE_NAME "scheduler"
#define PROCESS_IMAGE_NAME "process"
#define bool char
#define true 1
#define false 0
#define new(a) (a*)malloc(sizeof(a))
#define delete(a) free a
#endif //OSREQUIRMENT_DEFS_H
