#include <srtn_algorithm.h>
#include <srtn_queue.h>
#include <clk_utilities.h>
#include <queue_utilities.h>

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

process_data* running_process;
int last_time;
bool done_receiving;

void stop_running_process()
{
    if (running_process == NULL)
        return;
    kill(running_process->pid, SIGSTOP);
    running_process->remaining_time -= last_time - getClk();
    running_process->state = STOPPED;
    srtn_queue_push(running_process);
    printf("@T=%i SRTN: stopped %i\n", getClk(), running_process->process.id);
    running_process = NULL;
}

void run_next_process()
{
    running_process = srtn_queue_pop();
    int clk = getClk();
    if (running_process->pid == -1)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child
            char  remaining_time_buffer[12]; // NOTE Why 12?
            sprintf(remaining_time_buffer , "%d" , running_process->remaining_time);
            if(execl(PROCESS_IMAGE_NAME, PROCESS_IMAGE_NAME, remaining_time_buffer, (char*)NULL) == -1)
            {
                perror("Error while process creation");
                exit(1);
            }
        }
        // Parent
        running_process->pid = pid;
        running_process->state = STARTED;
        running_process->start_time = clk;
        printf("@T=%i SRTN: started %i\n", getClk(), running_process->process.id);
    }
    else
    {
        kill (running_process->pid, SIGCONT);
        running_process->state = RESUMED;
        printf("@T=%i SRTN: resumed %i\n", getClk(), running_process->process.id);
    }
    last_time = clk;
}

void sigusr1_handler(int signum)
{
    stop_running_process();
    process_struct * received_process = (process_struct*)malloc(sizeof(process_struct));
    int msg_code;
    do
    {
        msg_code = Recmsg(received_process);
        if (msg_code == 0)
        {
            printf("@T=%i SRTN: received %i\n", getClk(), received_process->id);
            srtn_queue_push(process_data_init(received_process));
        }
        else if (msg_code == 1)
        {
            done_receiving = true;
        }
        else if (msg_code != -1)
        {
            perror("Should never print this message");
            abort();
        }
    } while (msg_code == 0);
    // TODO Free received_process
    run_next_process();
}

void sigchld_handler(int signum)
{
    signal(SIGUSR1, SIG_IGN);
    pid_t pid;
    int status;
    if (pid = waitpid(-1, &status, WNOHANG))
    {
        printf("@T=%i SRTN: finished %i\n", getClk(), running_process->process.id);
        running_process->remaining_time = 0;
        running_process->finish_time = getClk();
        running_process->state = FINISHED;
        // log running_process
        // save running_process somewhere or free it
        if (running_process->pid == pid)
        {
            running_process = NULL;
        }
        else
        {
            perror("Should never print this message");
            abort();
        }
    }
    signal(SIGUSR1, sigusr1_handler);
    raise(SIGUSR1);
}

void shortest_remaining_time_next()
{
    srtn_queue_init();
    running_process = NULL;
    last_time = 0;
    done_receiving = false;
    
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGCHLD, sigchld_handler);
    
    while (!done_receiving || !srtn_queue_empty())
        pause();

    // LOG CPU utilization, Avg WTA, Avg Waiting, Std WTA
}