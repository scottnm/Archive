BinaryTree.o: BinaryTree.c
		gcc -g -c BinaryTree.c

clean:
	rm -rf *.o a.out
