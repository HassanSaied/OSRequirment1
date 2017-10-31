#include <srtn_queue.h>

#include <red_black_tree.h>

rb_red_blk_tree * tree;

void srtn_queue_init(void)
{
    tree = RBTreeCreate(compare, NullFunction, NullFunction, print, NullFunction);
}

bool srtn_queue_empty(void)
{
    return tree->root->left == tree->nil; // Needs check
}

void srtn_queue_push(process_data * data)
{
    RBTreeInsert(tree, &(data->remaining_time), data);
}

process_data * srtn_queue_pop(void)
{
    rb_red_blk_node * current_node = tree->root->left;
    while(current_node->left != tree->nil)
        current_node = current_node->left;
    process_data* process_data_ptr = (process_data*) current_node->info;
    RBDelete(tree, current_node);
    return process_data_ptr;
}

void srtn_queue_destroy(rb_red_blk_tree *tree)
{
    RBTreeDestroy(tree);
}
