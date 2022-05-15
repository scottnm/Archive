/*** INTRODUCTION
 *
 * Oh lord, here we go... pointers...
 *
 * This is the real defining characteristic of lower-level languages.
 *      and people often have trouble grasping the concept of pointers.
 *      Pointers introduce the concept of manually managing our programs
 *      memory usage.
 *      You will need to play around with them a bit yourself before
 *      you understand this. As always feel free to ask me questions
 *      if you don't understand anything.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

// Increments the value pointed to by the pointer
void increment_value_pointed_to(int* integer_pointer);

typedef struct simple_struct
{
    double d;
    float f;
    int n;
    char c;
} simple_struct;

void main(int argc, char** argv)
{
    /*
     * To begin a discussion of pointers, we need to begin with a quick
     *      discussion of memory. When you write "int x = 5;" in a Java
     *      program, you are telling the computer to make room to store
     *      an integer value, that you want to name that storage "x",
     *      and that you want to store the value 5 there.
     *
     * This storage is what we call "memory" (not to be confused
     *      with a hard drive/disk).
     *
     */
    int integer = 5;

    /* 
     * Where a value is stored in memory is called an "address"
     * A pointer is a variable that holds one of these addresses.
     *
     * To declare a pointer to a value of type T, you need
     * declare the pointer variable with type 
     */
    int* integer_pointer;

    /* Use the "&" operator to get the address of a variable */
    integer_pointer = &integer;

    /* See misc note #1 for an explanation of hexadecimal */
    printf("value @ __%p__ before increment is __%d__\n",
            integer_pointer, integer);
    
    /*
     * Passing around these pointers is similar to passing
     *      around object references in Java. If you pass
     *      an object to a method in Java and modify it
     *      it modifies all variables that held that
     *      object reference. If you pass a pointer
     *      and modify the value it points to, it
     *      will update the original value and all
     *      pointers pointing to that value will
     *      see that change.
     *
     */
    increment_value_pointed_to(integer_pointer);

    printf("value @ __%p__ after  increment is __%d__\n",
            integer_pointer, integer);

    /* You can also make pointers for structs as well */
    simple_struct my_struct;
    my_struct.d = 0.0;
    my_struct.f = 1.0;
    my_struct.n = 2;
    my_struct.c = '3';
    simple_struct* my_struct_pointer = &my_struct;

    /*
     * Just like normal pointers, you can dereference struct
     * pointers, but with struct pointers instead of just
     * using a '*' you use the arrow operator
     */
    printf("my_struct.d = %f\n", my_struct.d);
    my_struct_pointer->d = 1000.0;
    printf("my_struct.d = %f\n", my_struct.d);

    /*
     * Think of using pointers similarly to how objects kind of
     * work in java.
     *
     * There is a special function we have to call
     *      to generate the data that a pointer refers to.
     *      (new in Java, malloc in C)
     *
     * To use malloc you must...
     *      1.  #include <stdlib.h>
     *      2a. call malloc with the number of bytes you want to allocate
     *      2b. use sizeof(<insert type here>) function to avoid constants
     *      3.  cast the return value as the appropriate pointer type
     */
    simple_struct* my_struct_ptr_2 = (simple_struct*)malloc(sizeof(simple_struct));
    my_struct_ptr_2->d = 0.0;
    my_struct_ptr_2->f = 1.0;
    my_struct_ptr_2->n = 2;
    my_struct_ptr_2->c = '3';

     /*
     *
     * The biggest difference is that in Java, the memory
     *      associated with these objects is managed by the
     *      garbage collector. For those unfamiliar, the 
     *      garbage collector is a magic memory fairy that
     *      makes sure you don't run out of memory when running
     *      a Java program that instantiates many objects.
     *         
     * In C, we have to manage this memory ourselves. No
     *      garbage collector (which can be a good or a bad
     *      thing depending on how you look at it).
     *          
     * When you are done with the value pointed to by a pointer
     *      that you got back from malloc, use the free function
     *      to free up that memory
     *
     * CAUTION:
     *      DO NOT FREE A POINTER MORE THAN ONCE
     *      DO NOT FORGET TO FREE A POINTER
     *      DO NOT USE UNITIALIZED POINTERS
     *          the following will crash...
     *          int* my_ptr;
     *          *my_ptr = 5;
     */
    free(my_struct_ptr_2);

    // see misc note #2 for explanation of segfaults
}


void increment_value_pointed_to(int* integer_pointer)
{
    /*
     * Pointers can be "dereferenced" to access the value stored
     *      at the address held by the pointer
     *
     *
     * *integer_pointer = some_value;
     * says to write/store some_value at the address pointed
     *      to by integer_pointer 
     *
     * int some_var = *integer_pointer;
     * says to read/load the value at the address pointed to
     *      by integer_pointer
     */

    /*
     * this will read the value pointed to by integer_pointer,
     *      increment it, and store it back in memory
     */
    *integer_pointer = *integer_pointer + 1;
}


/*** SUMMARY
 * C is a unique language in that in lets you manage your memory by hand
 *
 * A pointer is a variable that holds an address in memory
 *
 * Pointers can be declared to any kind of type including structs
 *
 * You can dereference pointers to access the value at that address in memory
 *
 * You can use malloc to allocate memory for values that you should exist for
 *      awhile like objects in Java
 *
 * Use free to free up that memory
 */


/** MISC NOTES
 * 1. Hexadecimal
 *
 * Note: when the printfs print out the integer pointer, it will print out
 * it's value as a hexadecimal number. Hexadecimal for those unfamiliar
 * is just another number-base similar to binary, which is base-2, and
 * decimal, which is base-10; hexadecimal is base-16 so to count in
 * hex you count 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F, 10,
 * 11, 12, 13, 14, 15, 16, 17, 18, 19, 1A, 1B, 1C, etc..
 *
 * hexadecimal numbers are usually presented as 0x<number>
 *
 * 2. Seg fault
 *
 * Eventually when you are programming in C you are going to write some beautiful
 * code that compiles, but when you run it you get output saying that your program
 * has experienced a segmentation fault or segfault. This is an indicator that your
 * program has accessed memory that it is not allowed to access. Here are a few common
 * examples of code that will crash with a seg fault in C
 *
 *      int arr[4]; // initialize an array of size 4
 *      arr[4] = 10; // try and set the 5th element. Out of bounds & segfault
 *
 *      int* ip;    // create uninitialized pointer, what is it pointing to? undefined
 *      *ip = 5;    // try to dereference the uninitialized pointer. segfault
 *                  // similar to a null reference exception in java
 */
