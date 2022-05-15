#include <stdio.h>
#include <stdlib.h>
#include "binarytree.h"

#define CREATE_BT_NODE (bt_node*)(malloc(sizeof(bt_node)))

typedef int bool;
#define true 1
#define false 0

static void init_add(binary_tree* bt, int val); 
static void bt_add_helper(linked_list* order_list, bt_node* node, int val);
static void bt_print_helper(bt_node* node, int spaces);
static void bt_delete_helper(bt_node* node);

#define INDEX_ERR -1
static int order(linked_list* order_list, int val_a, int val_b);
static int index_of(linked_list* order_list, int val);

binary_tree* new_binary_tree(linked_list* in_order, linked_list* post_order) {
    binary_tree* new_bt = (binary_tree*)(malloc(sizeof(binary_tree)));
    new_bt->root = NULL;
    new_bt->order = in_order;
    init_add(new_bt, post_order->head->val);
    int s = post_order->size;
    ll_node* llnode = post_order->head->next;
    for(; s > 1; --s) {
        bt_add(new_bt, llnode->val);
        llnode = llnode->next;
    }
    return new_bt;
}
    


void bt_add(binary_tree* bt, int val) {
    bt_add_helper(bt->order, bt->root, val);
}

static void init_add(binary_tree* bt, int val) {
    bt_node* node = CREATE_BT_NODE;
    node->left = NULL;
    node->right = NULL;
    node->val = val;
    bt->root = node;
}

static void bt_add_helper(linked_list* order_list, bt_node* node, int val) {
    int comp = order(order_list, val, node->val);
    if (comp < 0) {
        if (node->left == NULL) {
            bt_node* new_node = CREATE_BT_NODE;
            new_node->left = NULL;
            new_node->right = NULL;
            new_node->val = val;
            node->left = new_node;
        }
        else {
            bt_add_helper(order_list, node->left, val);
        }
    }
    else if (comp > 0) {
        if (node->right == NULL) {
            bt_node* new_node = CREATE_BT_NODE;
            new_node->left = NULL;
            new_node->right = NULL;
            new_node->val = val;
            node->right = new_node;
        }
        else {
            bt_add_helper(order_list, node->right, val);
        }
    }
}

static int order(linked_list* order_list, int val_a, int val_b) {
    if (val_a == val_b) {
        return 0;
    }

    int a_index = index_of(order_list, val_a);
    int b_index = index_of(order_list, val_b);
    if (a_index < b_index) {
        return -1;
    }
    else {
        return 1;
    }
}

static int index_of(linked_list* order_list, int val) {
    ll_node* node = order_list->head;
    int s = order_list->size;
    int i = 0;
    for(; i < s; ++i) {
        if (node->val == val) {
            return i;
        }
        node = node->next;
    }
    return INDEX_ERR;
}

void bt_print(binary_tree* bt) {
    bt_print_helper(bt->root, 0);
}

static void bt_print_helper(bt_node* node, int spaces) {
    if (node == NULL) {
        return;
    }

    bt_print_helper(node->right, spaces + 4);

    int i = 0;
    for(; i < spaces; ++i) { printf(" "); }
    printf("%d\n", node->val);

    bt_print_helper(node->left, spaces + 4);
}

void bt_delete(binary_tree* bt) {
    bt_delete_helper(bt->root);
}

static void bt_delete_helper(bt_node* node) {
    bool has_left = node->left != NULL;
    bool has_right = node->right != NULL;

    if (!(has_left || has_right)) {
        free(node);
        return;
    }

    if (has_left) {
        bt_delete_helper(node->left);
    }

    if (has_right) {
        bt_delete_helper(node->right);
    }

    free(node);
}
