CC      := gcc
CFLAGS  := -Wall -g
FILES   := main.c       \
		   linkedlist.c \

main: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o BinaryTreeBuilder

clean:
	rm -f BinaryTreeBuilder
