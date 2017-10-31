//
// Created by hassan on 10/30/17.
//

#include <hpf_red_black_tree.h>


rb_red_blk_tree * hpf_init_tree(void)
{
    return RBTreeCreate(compare , NullFunction , NullFunction,print , NullFunction);
}

rb_red_blk_node * hpf_tree_insert(rb_red_blk_tree *tree, process_data *data)
{
    return RBTreeInsert(tree , &(data->process.priority) ,data);
}

process_data * hpf_tree_delete(rb_red_blk_tree *tree, rb_red_blk_node *node)
{
    process_data * deleted_node =(process_data*) node ->info;
    RBDelete(tree , node);
    return deleted_node;
}

rb_red_blk_node * hpf_get_next_process(rb_red_blk_tree *tree)
{
    rb_red_blk_node * current_node = tree->root->left;
    while(current_node->right != tree->nil)
        current_node = current_node->right;
    return current_node;
}
bool hpf_tree_empty(rb_red_blk_tree * tree)
{
    return tree->root->left == tree->nil;
}
void hpf_tree_destroy(rb_red_blk_tree *tree)
{
    RBTreeDestroy(tree);
}

