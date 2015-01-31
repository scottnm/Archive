#ifndef MAIN
#define MAIN
#include "BinaryTree.h"

#endif

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("ERROR: too few arguments\n");
		return 0;
	} 
	
	node* root = add(NULL, atoi(argv[1]));

	int i;
	for(i = 2; i < argc; i++)
		add(root, atoi(argv[i]));

	printTree(root);	
}
