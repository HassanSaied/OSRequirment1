//
// Created by hassan on 10/30/17.
//

#ifndef OSREQUIRMENT_SRTN_RED_BLACK_TREE_H
#define OSREQUIRMENT_SRTN_RED_BLACK_TREE_H

#include <red_black_tree.h>
#include <process_data.h>

rb_red_blk_tree * srtn_init_tree(void);
rb_red_blk_node * srtn_tree_insert(rb_red_blk_tree *tree, process_data *data);

process_data * srtn_tree_delete(rb_red_blk_tree *tree, rb_red_blk_node *node);

rb_red_blk_node * srtn_get_next_process(rb_red_blk_tree *tree);
void srtn_tree_destroy(rb_red_blk_tree *tree);

#endif //OSREQUIRMENT_SRTN_RED_BLACK_TREE_H
