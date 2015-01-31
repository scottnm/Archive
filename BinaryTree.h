#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct node{
	int data;
	struct node* left;
	struct node* right;
};

typedef struct node node;
/**
Purpose: 
Add the value to the current tree defined by it's node root

Param:
current_node: pointer to the root of the tree
value: value to be appended to the tree
*/
node* add(node* curr_node, int value);

/**
Purpose:
Remove the value from the tree defined by it's node root

Param:
curr_node: pointer to the current_node of the tree
value: value to be removed
*/
node* removeNode(node* curr_node, int value);
 
/**
Purpose:
Check if the tree defined by curr_node contains the value

Param:
curr_node: pointer to the current node to check
value: value to search the tree for
*/
int contains(node* curr_node, int value);

/**
HELPER FOR PRINTTREE

ACTUAL PRINTTREE METHOD
*/
void printTree(node* root);
void printTreeRecursively(node* root, int spaces);

int max(node* root);
#endif


