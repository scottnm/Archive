
typedef struct Node Node;
typedef struct BinaryTree Tree;

struct Node
{
	Node *left;
	Node *right;
	int data;
};

struct BinaryTree
{
	Node *root;
	int size;
};