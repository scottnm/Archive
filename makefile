build: BinaryTree.o main.o
		gcc -g BinaryTree.o main.o

BinaryTree.o: BinaryTree.c
		gcc -g -c BinaryTree.c

clean:
	rm -rf *.o a.out
