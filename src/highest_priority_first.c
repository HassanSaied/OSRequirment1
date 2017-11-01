#include <highest_priority_first.h>

#include <defs.h>
#include <queue_utilities.h>
#include <process_data.h>
#include <process_struct.h>
#include <clk_utilities.h>
#include <logger.h>

void highest_priority_first()
{
    /*
     * To implement highest priority first, do the following
     * 1) while(msg_queue not empty)--> get processes, insert into tree
     * 2) get next process from tree
     * 3) run the process and wait till returns
     * 4) print stats
     * 6) return to 1 till finished
     * 7) print final stats
     */

    bool last_received_process = false;
    hpf_init_tree();
    while (!last_received_process || !hpf_tree_empty()) {
        int rec_msg_code;
        process_struct * current_received_process = (process_struct*)malloc(sizeof(process_struct));
        //wait till something arrives
        do
        {
            rec_msg_code = Recmsg(current_received_process);
        }while (rec_msg_code == -1 && hpf_tree_empty());
        //keep reading while there is something
        while (rec_msg_code == 0)
        {
            hpf_tree_insert(process_data_init((current_received_process)));
            /*printf("Received Process With id: %d, Priority: %d, RunningTime: %d, at Time: %d\n",
                   current_received_process->id , current_received_process->priority , current_received_process->runningTime,
                   current_received_process->arrivalTime);*/
            rec_msg_code = Recmsg(current_received_process);
        }
        free(current_received_process);
        //here rec_msg_code, can either be 1 or -1, if 1 then I have no more processes, if -1 then there is more
        last_received_process |= rec_msg_code == 1;
        rb_red_blk_node * current_node= hpf_get_next_process();
        process_data * current_process = (process_data*)current_node->info;
        int current_process_pid = fork();
        if(current_process_pid == 0)//child
        {
            char  remaining_time_buffer[12];
            sprintf(remaining_time_buffer , "%d" , current_process->remaining_time);
            if(execl(PROCESS_IMAGE_NAME , PROCESS_IMAGE_NAME ,remaining_time_buffer, (char*)NULL)==-1)
            {
                perror("Couldn't Create Process ");
                exit(1);
            }

        } else //parent
        {
            current_process->start_time = getClk();
            current_process->state = STARTED;
            /*printf("At time %d process %d started arr %d total %d remain %d wait %d\n",
                    getClk(),current_process->process.id,current_process->process.arrivalTime,
            current_process->process.runningTime,current_process->remaining_time,0);*/
            logger_log(current_process);
            int stat;
            waitpid(current_process_pid , &stat , 0);
            current_process->remaining_time=0;
            current_process->finish_time = getClk();
            current_process->state = FINISHED;
            /*printf("At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
                   getClk(),current_process->process.id,current_process->process.arrivalTime,
                   current_process->process.runningTime,current_process->remaining_time,0,TA,
                   (double)TA/current_process->process.runningTime);*/
            logger_log(current_process);
            hpf_tree_delete(current_node);
            process_data_destroy(current_process);
           /* printf("Deleted:At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
                   getClk(),current_process->process.id,current_process->process.arrivalTime,
                   current_process->process.runningTime,current_process->remaining_time,0,TA,
                   (double)TA/current_process->process.runningTime);*/
            //Decide whether I want to do any thing with the process or not
        }


    }
}
