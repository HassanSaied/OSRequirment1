//
// Created by hassan on 11/1/17.
//
#include <double_queue.h>
double_queue* double_queue_init()
{
    double_queue* queue = (double_queue*)malloc(sizeof(double_queue));
    queue->head = init_generic_queue();
    return queue;
}

bool double_queue_enqueue(double_queue *queue, double new_node)
{
    double * new_node_pointer = (double *) malloc((sizeof(int)));
    *new_node_pointer = new_node;
    return generic_queue_enqueue(queue->head , new_node_pointer);
}
double double_queue_dequeue(double_queue *queue)
{
    double * returned_value_pointer = (double *)generic_queue_dequeue(queue->head);
    double returned_value = *returned_value_pointer;
    free(returned_value_pointer);
    return returned_value;
}
bool double_queue_empty(double_queue *queue)
{
    return  generic_queue_empty(queue->head);
}
void double_queue_destroy(double_queue *queue)
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