#include "BinaryTree.h"

node* add(node* curr_node, int value){
	/*
	base case: current node is null
	allocate space for new node and add
	*/
	if(!curr_node){
		node* new_node = (node*)malloc(sizeof(node));
		new_node -> data = value;
		new_node -> left = NULL;
		new_node -> right = NULL;
		return new_node;
	}

	int curr_val = curr_node -> data;
	if(value < curr_val)
		curr_node->left = add(curr_node->left, value);
	else if(value > curr_val)
		curr_node->right = add(curr_node->right, value);

	return curr_node;
}

node* removeNode(node* curr_node, int value){
	if(!curr_node) // node to remove not found
		return NULL;
	
	int curr_val = curr_node -> data;
	if(value < curr_val)
		curr_node->left = removeNode(curr_node->left, value);

	else if(value > curr_val)
		curr_node->right = removeNode(curr_node->right, value);
	
	else{
		
		if(!(curr_node->left || curr_node->right)){ // if leaf node
			free(curr_node);
			return NULL;
		}
		
		if(!(curr_node->left && curr_node->right)){ // has one child
			if(!curr_node->left){
				node* right_n = curr_node->right;
				free(curr_node);
				return right_n;
			}
			
			else{
				node* left_n = curr_node->left;
				free(curr_node);
				return curr_node->left;
			}
		}
		
		else{
			int max_of_left = max(curr_node->left);
			curr_node->left = removeNode(curr_node->left, max_of_left);
			curr_node->data = max_of_left;
		}
	}
	
	return curr_node;
}

int max(node* root){
	if(!root)
		return INT_MIN;
	
	while(!root->right) //until right rightmost node
		root = root->right;

	return root->data;
}

int contains(node* curr_node, int value){
	while(curr_node){
		int curr_val = curr_node->data;

		if(value < curr_val)
			curr_node = curr_node->left;

		else if(value > curr_val)
			curr_node = curr_node->right;
		
		else
			return TRUE;
	}

	return FALSE;
}

void printTree(node* root){
	printTreeRecursively(root, 0);
}

void printTreeRecursively(node* root, int spaces){
	if(root){
		printTreeRecursively(root->right, ++spaces);
		
		int i;
		for(i = 0; i < spaces; i++)
			printf(" ");
		printf("%d\n", root->data);
		
		printTreeRecursively(root->left, ++spaces);
	}
}
