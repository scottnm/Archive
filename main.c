#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "binarytree.h"
#include "linkedlist.h"

#define INPUT_ARG 1
#define FILE_READ_ONLY "r"

typedef int bool;
#define true 1
#define false 0

static inline int char_to_num(int n);
static linked_list* build_list_from_file(FILE* file, bool reverse);

int main(int argc, char** argv) {
    if (argc < (INPUT_ARG + 1)) {
        printf("***Missing input argument\n");
        return EXIT_FAILURE; 
    }

    FILE* file = fopen(argv[INPUT_ARG], FILE_READ_ONLY);
    if (!file) {
        printf("***File \"%s\" could not be found\n", argv[INPUT_ARG]);
        fclose(file);
        return EXIT_FAILURE;
    }

    linked_list* in_order_list = build_list_from_file(file, false);
    linked_list* post_order_list = build_list_from_file(file, true);
    if (in_order_list->size != post_order_list->size ||
        in_order_list->size == 0 || post_order_list->size == 0) {
        ll_delete(in_order_list);
        ll_delete(post_order_list);
        fclose(file);
        printf("***the length of the inorder and post order traversal"
               " in the input must be nonzero and equal.");
        return EXIT_FAILURE;
    }

    printf("inorder: ");
    ll_print(in_order_list);
    printf("postorder: ");
    ll_print(post_order_list);

    binary_tree* btree = new_binary_tree(in_order_list, post_order_list);
    bt_print(btree);
    
    fclose(file);
    ll_delete(in_order_list);
    free(in_order_list);
    ll_delete(post_order_list);
    free(post_order_list);
    bt_delete(btree);
    free(btree);
    return EXIT_SUCCESS;
}

int char_to_num(int n) {
    assert(n >= 48 && n <= 57);
    return n - 48;
}

#define COMMA 44
#define SPACE 32
#define NEWLINE 10
#define RETURN 13
linked_list* build_list_from_file(FILE* file, bool reverse) {
    linked_list* new_list = new_linked_list();
    int ch;
    bool still_reading = true;
    bool first_val_read = false;
    int number = 0;
    while (!first_val_read && still_reading && (ch = getc(file)) != EOF) {
        switch(ch) {
            case SPACE:
                break; // ignore spaces
            case RETURN:
            case NEWLINE:
                still_reading = false;
                break;
            case COMMA:
                ll_init_add(new_list, number);
                number = 0;
                first_val_read = true;
                break;

            // NUMBER ENCOUNTERED
            default: 
                number = number*10 + char_to_num(ch); 
        }
    }
    while (still_reading && (ch = getc(file)) != EOF) {
        switch(ch) {
            case SPACE:
                break; // ignore spaces
            case RETURN:
            case NEWLINE:
                still_reading = false;
                break;
            case COMMA:
                if (reverse) {
                    ll_add_to_head(new_list, number);
                }
                else {
                    ll_add_to_tail(new_list, number);
                }
                number = 0;
                break;

            // NUMBER ENCOUNTERED
            default: 
                number = number*10 + char_to_num(ch); 
        }
    }
    if (reverse) {
        ll_add_to_head(new_list, number);
    }
    else {
        ll_add_to_tail(new_list, number);
    }
    return new_list;
}
