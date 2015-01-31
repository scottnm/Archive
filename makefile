build2: main.c BinaryTree.c
		gcc -g main.c BinaryTree.c -o main

build: BinaryTree.o main.o
		gcc -g BinaryTree.o main.o

BinaryTree.o: BinaryTree.c
		gcc -g -c BinaryTree.c

main.o: main.c
		gcc -g -c main.c

clean:
	rm -rf *.o a.out
