/**
 *
 * IMPORTANT
 * implement all of these functions in a file called linked_list.c
 */

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stdbool.h>   // allows C to use bool-like values

typedef struct node
{
    int value;
    struct node* next;
} node;

/* allocates a node and returns a pointer to it */
node* node_create (int val);



typedef struct linked_list
{
    node* head;
    int size;
} linked_list;

/* returns an initialized linked list */
linked_list ll_init (void);

/* adds an element to the end of the linked list */
linked_list ll_add (linked_list list, int val);

/* insert an element in the linked list at the given position */
linked_list ll_insert (linked_list list, int val, int pos);

/* remove the first occurence of the value from the linked list */
linked_list ll_remove_first (linked_list list, int val);

/* print the linked list */
void ll_print (linked_list list);

/* compares two linked lists and returns whether they are equal */
bool ll_equals (linked_list list_a, linked_list list_b);

/* frees all of the memory used by the linked list */
void ll_destroy (linked_list list);

#endif // __LINKED_LIST_H__
