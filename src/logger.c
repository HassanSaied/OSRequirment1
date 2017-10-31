#include <logger.h>

#include <clk_utilities.h>
#include <stdio.h>

FILE * log_file;

void logger_init()
{
    log_file = fopen("scheduler.log", "w");
}

void logger_log(process_data *data)
{
    int clk = getClk();
    int id = data->process.id;
    int arr = data->process.arrivalTime;
    int total = data->process.runningTime;
    int remain = data->remaining_time;
    int wait = clk - arr - total + remain;
    int TA;
    switch (data->state)
    {
        case STOPPED:
            fprintf(log_file, "At time %d process %d stopped arr %d total %d remain %d wait %d\n", clk, id, arr, total, remain, wait);
            break;
        case STARTED:
            fprintf(log_file, "At time %d process %d started arr %d total %d remain %d wait %d\n", clk, id, arr, total, remain, wait);
            break;
        case RESUMED:
            fprintf(log_file, "At time %d process %d resumed arr %d total %d remain %d wait %d\n", clk, id, arr, total, remain, wait);
            break;
        case FINISHED:
            TA = data->finish_time - arr;
            if(!(TA % total))
                fprintf(log_file, "At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %d\n", clk, id, arr, total, 0, TA - total, TA, TA / total);
            else
                fprintf(log_file, "At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n", clk, id, arr, total, 0, TA - total, TA, (double) TA / total);
            break;
        default:
            perror("Allowed states: started, resumed, stopped, finished");
            abort();
    }
}

void logger_destroy()
{
    fclose(log_file);
}
