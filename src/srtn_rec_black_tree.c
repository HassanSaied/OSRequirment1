//
// Created by hassan on 10/30/17.
//
#include <srtn_red_black_tree.h>

rb_red_blk_tree * srtn_init_tree(void)
{
    return RBTreeCreate(compare , NullFunction , NullFunction
            ,print , NullFunction);
}

rb_red_blk_node * srtn_tree_insert(rb_red_blk_tree *tree, process_data *data)
{
    return RBTreeInsert(tree , &(data->remaining_time) ,data);
}

process_data * srtn_tree_delete(rb_red_blk_tree *tree, rb_red_blk_node *node)
{
    process_data * deleted_node =(process_data*) node ->info;
    RBDelete(tree , node);
    return deleted_node;
}

rb_red_blk_node * srtn_get_next_process(rb_red_blk_tree *tree)
{
    rb_red_blk_node * current_node = tree->root->left;
    while(current_node->left != tree->nil)
        current_node = current_node->left;
    return current_node;
}
void srtn_tree_destroy(rb_red_blk_tree *tree)
{
    RBTreeDestroy(tree);
}
