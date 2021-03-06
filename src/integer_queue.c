//
// Created by hassan on 11/1/17.
//
#include <integer_queue.h>
integer_queue* integer_queue_init()
{
    integer_queue* queue = (integer_queue*)malloc(sizeof(integer_queue));
    queue->head = init_generic_queue();
    return queue;
}

bool integer_queue_enqueue(integer_queue *queue, int new_node)
{
    int * new_node_pointer = (int*) malloc((sizeof(int)));
    *new_node_pointer = new_node;
    return generic_queue_enqueue(queue->head , new_node_pointer);
}
double integer_queue_dequeue(integer_queue *queue)
{
    int * returned_value_pointer = (int *)generic_queue_dequeue(queue->head);
    int returned_value = *returned_value_pointer;
    free(returned_value_pointer);
    return returned_value;
}
bool integer_queue_empty(integer_queue *queue)
{
    return  generic_queue_empty(queue->head);
}
void integer_queue_destroy(integer_queue *queue)
{
    generic_queue_node * current_node = queue->head->first;
    while (current_node!=NULL)
    {
        int * current_int = current_node->data;
        free(current_int);
        current_node = current_node->next;
    }
    generic_queue_destroy(queue->head);
    free(queue);
}