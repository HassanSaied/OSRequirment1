#include <generic_queue.h>

generic_queue_head * init_generic_queue()
{
    generic_queue_head * queue_head = (generic_queue_head *) malloc(sizeof(generic_queue_head));
    queue_head->first = NULL;
    queue_head->last = NULL;
}

bool generic_queue_enqueue(generic_queue_head * head, void * new_node)
{
    generic_queue_node * new_queue_node = (generic_queue_node *) malloc(sizeof(generic_queue_node));
    if (new_queue_node == NULL)
    {
        return false;
    }
    new_queue_node->data = new_node;
    new_queue_node->next = NULL;
    if (generic_queue_empty(head))
    {
        head ->first = new_queue_node;
    }
    else
    {
        head->last->next = new_queue_node;
    }
    head->last = new_queue_node;
}

void * generic_queue_dequeue(generic_queue_head * head)
{
    if(generic_queue_empty(head)) return NULL;
    generic_queue_node * current_first_node  = head->first;
    head->first = head->first->next;
    return current_first_node->data;
}

bool generic_queue_empty(generic_queue_head * head)
{
    return head->first == NULL;
}
