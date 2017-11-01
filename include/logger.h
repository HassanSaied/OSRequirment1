#ifndef OSREQUIRMENT_LOGGER_H
#define OSREQUIRMENT_LOGGER_H

#include <process_data.h>

void logger_init();
void logger_log(process_data * data);
<<<<<<< HEAD
void logger_print_perf_file(int free_time);
=======
void logger_print_perf_file(int current_time);
>>>>>>> 2fcc44851ecd134c571cf6836d9f7d54374da7d8
void logger_destroy();

#endif //OSREQUIRMENT_LOGGER_H
 
