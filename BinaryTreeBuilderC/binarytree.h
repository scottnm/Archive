#ifndef __B_TREE_H__
#define __B_TREE_H__

#include "linkedlist.h"

struct bt_node {
    struct bt_node* left;
    struct bt_node* right;
    int val;
};
typedef struct bt_node bt_node;

struct binary_tree {
    bt_node* root;
    linked_list* order;
};
typedef struct binary_tree binary_tree;

binary_tree* new_binary_tree(linked_list* in_order, linked_list* post_order);
void bt_add(binary_tree* bt, int val);
void bt_print(binary_tree* bt); 
void bt_delete(binary_tree* bt);

#endif  // __B_TREE_H__
