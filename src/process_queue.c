#include <process_queue.h>

process_queue * init()
{
    process_queue * queue = (process_queue *)malloc(sizeof(process_queue));
    queue->head = init_generic_queue();
}

bool enqueue(process_queue * queue, process_struct * new_node)
{
    return generic_queue_enqueue(queue->head , (void *)new_node);
}

process_struct * dequeue(process_queue * queue)
{
    return (process_struct *)generic_queue_dequeue(queue->head);
}

bool empty(process_queue * queue)
{
    return generic_queue_empty(queue->head);
}
void destroy(process_queue * queue)
{
    generic_queue_destroy(queue->head);
    free(queue);
}
