#include <highest_priority_first.h>

#include <defs.h>
#include <queue_utilities.h>
#include <process_data.h>
#include <process_struct.h>
#include <clk_utilities.h>

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
     *
     */

    bool last_received_process = false;
    rb_red_blk_tree* tree =  hpf_init_tree();
    while (!last_received_process || !hpf_tree_empty(tree)) {
        int rec_msg_code;
        process * current_received_process = (process*)malloc(sizeof(process));
        //wait till something arrives
        do
        {
            rec_msg_code = Recmsg(current_received_process);
        }while (rec_msg_code == -1 && hpf_tree_empty(tree));
        //keep reading while there is something
        while (rec_msg_code == 0)
        {
            hpf_tree_insert(tree , init_process_data((current_received_process)));
            /*printf("Received Process With ID: %d, Priority: %d, RunningTime: %d, at Time: %d\n",
                   current_received_process->ID , current_received_process->priority , current_received_process->runningTime,
                   current_received_process->arrivalTime);*/
            rec_msg_code = Recmsg(current_received_process);
        }
        //here rec_msg_code, can either be 1 or -1, if 1 then I have no more processes, if -1 then there is more
        last_received_process |= rec_msg_code == 1;
        rb_red_blk_node * current_node= hpf_get_next_process(tree);
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
            current_process->state = RUNNING;
            printf("At time %d process %d started arr %d total %d remain %d wait %d\n",
                    getClk(),current_process->inner_process.ID,current_process->inner_process.arrivalTime,
            current_process->inner_process.runningTime,current_process->remaining_time,0);
            int stat;
            waitpid(current_process_pid , &stat , 0);
            current_process->remaining_time=0;
            current_process->finish_time = getClk();
            current_process->state = FINISHED;
            int TA = current_process->start_time-current_process->inner_process.arrivalTime;
            printf("At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
                   getClk(),current_process->inner_process.ID,current_process->inner_process.arrivalTime,
                   current_process->inner_process.runningTime,current_process->remaining_time,0,TA,
                   (double)TA/current_process->inner_process.runningTime);
            hpf_tree_delete(tree,current_node);
            //Decide whether I want to do any thing with the process or not
        }


    }
}
