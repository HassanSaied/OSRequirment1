#include <srtn_queue.h>
#include <process_queue.h>
#include <stdio.h>

void read_process_from_file(process_queue * queue){
    FILE * processFile = fopen(PROCESS_FILE_NAME , "r");
    char lineIdentifier[10], ignoredEndLine;
    while (fscanf(processFile , "%s" , lineIdentifier) != EOF)
    {
        if (lineIdentifier[0] != '#')
        {
            //allocate a memory for new process
            process_struct * new_process = (process_struct *) malloc(sizeof(process_struct));
            if (new_process == NULL) //TODO: really must change this behaviour, 7mdy may kill me for this
            {
                continue;
            }
            new_process->id = atoi(lineIdentifier);
            fscanf(processFile , "%d%d%d%c" , &new_process->arrivalTime, &new_process->runningTime
                   , &new_process->priority, &ignoredEndLine);
            enqueue(queue , new_process);
        }
        else
        {
            char ignoredBuffer [80];
            fgets(ignoredBuffer , sizeof(ignoredBuffer) , processFile);
        }
    }
    fclose(processFile);
    return;
}

void print_queue(process_queue * queue)
{
    generic_queue_node * ptr = queue->head->first;
    do {
        process_struct * current_process = (process_struct*) ptr->data;
        printf("%d , %d , %d , %d\n" , current_process->id, current_process->arrivalTime,
               current_process->runningTime , current_process->priority);
        ptr = ptr->next;

    } while (ptr !=NULL);
}

void test_srtn_queue(process_queue * queue)
{
    srtn_queue_init();
    while(!empty(queue))
    {
        process_struct * current_process = dequeue(queue);
        srtn_queue_push(process_data_init(current_process));
    }
    while(!srtn_queue_empty())
    {
        enqueue(queue, &(srtn_queue_pop()->process));
    }
    print_queue(queue);
}

int main()
{
    process_queue * queue = init();
    read_process_from_file(queue);
    test_srtn_queue(queue);
    return 0;
}
