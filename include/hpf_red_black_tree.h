//
// Created by hassan on 10/30/17.
//

#ifndef OSREQUIRMENT_PROCESS_RED_BLACK_TREE_H
#define OSREQUIRMENT_PROCESS_RED_BLACK_TREE_H

#include <red_black_tree.h>
#include <process_data.h>
#include <defs.h>


rb_red_blk_tree * hpf_init_tree(void);
rb_red_blk_node * hpf_tree_insert(rb_red_blk_tree *tree, process_data *data);

process_data * hpf_tree_delete(rb_red_blk_tree *tree, rb_red_blk_node *node);

rb_red_blk_node * hpf_get_next_process(rb_red_blk_tree *tree);
bool hpf_tree_empty(rb_red_blk_tree * tree);
void hpf_tree_destroy(rb_red_blk_tree *tree);
#endif //OSREQUIRMENT_PROCESS_RED_BLACK_TREE_H
