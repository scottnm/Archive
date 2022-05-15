#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

#define CREATE_LL_NODE (ll_node*)(malloc(sizeof(ll_node)))

linked_list* new_linked_list(void) {
    linked_list* new_list = (linked_list*)(malloc(sizeof(linked_list)));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return new_list;
}

void ll_init_add(linked_list* list, int val) {
    ll_node* new_node = CREATE_LL_NODE;
    new_node->val = val;
    new_node->next = NULL;

    list->head = new_node;
    list->tail = new_node;
    list->size = 1;
}

void ll_add_to_head(linked_list* list, int val) {
    ll_node* new_node = CREATE_LL_NODE;
    new_node->val = val;
    new_node->next = list->head;
    list->head = new_node;
    ++list->size;
}

void ll_add_to_tail(linked_list* list, int val) {
    ll_node* new_node = CREATE_LL_NODE;
    new_node->val = val;
    list->tail->next = new_node;
    list->tail = new_node;
    ++list->size;
}

void ll_print(linked_list* list) {
    printf("[");
    int s = list->size;
    ll_node* node = list->head;
    for(; s > 1; --s) {
       printf("%d, ", node->val); 
       node = node->next;
    }
    if (s == 1) {
        // print the last one
        printf("%d]\n", node->val);
    }
}

void ll_delete(linked_list* list) {
    int s = list->size;
    for(; s > 0; --s) {
        ll_node* node = list->head;
        list->head = list->head->next;
        free(node);
    }
}
