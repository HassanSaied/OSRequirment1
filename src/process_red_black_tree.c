//
// Created by hassan on 10/30/17.
//

#include <process_red_black_tree.h>

int compare(const void* a,const void* b) {
    if( *(int*)a > *(int*)b) return(1);
    if( *(int*)a < *(int*)b) return(-1);
    return(0);
}
void print(const void * key)
{
    ;
}
void delete_process_data(void * data)
{
    free ((process_data*)data);
}
rb_red_blk_tree * init_process_tree(void)
{
    return RBTreeCreate(compare , NullFunction , delete_process_data
    ,print , NullFunction);
}

rb_red_blk_node * process_tree_insert(rb_red_blk_tree * tree , process_data * data)
{
    return RBTreeInsert(tree , &(data->inner_process.priority) ,data);
}

void process_tree_delete(rb_red_blk_tree * tree , rb_red_blk_node * node)
{
    return RBDelete(tree , node);
}

rb_red_blk_node * max_priority_process(rb_red_blk_tree *tree)
{
    rb_red_blk_node * current_node = tree->root->left;
    while(current_node->right != tree->nil)
        current_node = current_node->right;
    return current_node;
}
void process_tree_destroy(rb_red_blk_tree * tree)
{
    RBTreeDestroy(tree);
}

