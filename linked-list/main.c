#include "linked_list.h"
#include <assert.h>     // assert()
#include <stddef.h>     // NULL
#include <stdio.h>      // printf()

#define LIST_SIZE 5

int main()
{
    int list_a_elements[] = {1, 2, 3, 4, 5};
    int list_b_elements[] = {6, 1, 3, 2, 5};

    linked_list list_a = ll_init();
    linked_list list_b = ll_init();

    int i;
    for (i = 0; i < LIST_SIZE; ++i)
    {
        list_a = ll_add(list_a, list_a_elements[i]);
        list_b = ll_add(list_b, list_b_elements[i]);
    }

    assert( ! ll_equals(list_a, list_b) );

    list_a = ll_remove_first(list_a, 1);
    list_a = ll_remove_first(list_a, 2);
    list_b = ll_remove_first(list_b, 1);
    list_b = ll_remove_first(list_b, 2);

    list_a = ll_insert(list_a, 6, 0);
    list_b = ll_insert(list_b, 4, 2);

    printf("list a:\n");
    ll_print(list_a);

    assert( ll_equals(list_a, list_b) );

    ll_destroy(list_a);
    ll_destroy(list_b);

    printf("\nSUCCESS!\n");
}
