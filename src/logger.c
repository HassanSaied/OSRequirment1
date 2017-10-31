#include <logger.h>

#include <stdio.h>

FILE * log_file;

void logger_init()
{
    log_file = fopen("scheduler.log", "w");
}

void logger_log(process_data * data)
{
    // TODO
}

void logger_destroy()
{
    fclose(log_file);
}
