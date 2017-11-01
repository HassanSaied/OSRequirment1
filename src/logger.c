#include <logger.h>

#include <clk_utilities.h>
#include <stdio.h>
#include <math.h>
#include <double_queue.h>

FILE * log_file;
FILE * perf_file;
double_queue * wta_queue;
double total_wta = 0;
int total_time = 0;
int total_wait = 0;
int process_counter = 0;

void logger_init()
{
    log_file = fopen("scheduler.log", "w");
    perf_file = fopen("scheduler.perf","w");
    wta_queue = double_queue_init();
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
            double_queue_enqueue(wta_queue,(double) TA / total);
            total_wta += (double) TA / total;
            total_wait += wait;
            total_time += data->process.runningTime;
            process_counter += 1;
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
void logger_print_perf_file(int free_time)
{
    int curr_time = getClk();
    float cpu_utilization = (total_time+free_time) / (float)curr_time * 100;
    // double cpu_utilization = (total_time/(double)curr_time) * 100.0;
    double average_wta =  total_wta / process_counter;
    double average_wait = (double) total_wait / process_counter;
    double std_wta = 0;
    while (!double_queue_empty(wta_queue))
    {
        std_wta += pow((double_queue_dequeue(wta_queue)-average_wta),2.0);
    }
    std_wta/=process_counter;;
    std_wta = sqrt(std_wta);
    fprintf(perf_file , "CPU utilization=%.2f%%\n",cpu_utilization);
    fprintf(perf_file,"Avg WTA=%.2f\n",average_wta);
    fprintf(perf_file,"Avg Waiting=%.2f\n",average_wait);
    fprintf(perf_file,"Std WTA=%.2f\n",std_wta);

}
void logger_destroy()
{
    fclose(log_file);
    fclose(perf_file);
    double_queue_destroy(wta_queue);
}
