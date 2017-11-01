#ifndef OSREQUIRMENT_LOGGER_H
#define OSREQUIRMENT_LOGGER_H

#include <process_data.h>

void logger_init();
void logger_log(process_data * data);
void logger_print_perf_file(int current_time);
void logger_destroy();

#endif //OSREQUIRMENT_LOGGER_H
 
