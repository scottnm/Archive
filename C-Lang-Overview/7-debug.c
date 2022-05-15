/** Summary
 *
 * debugging GDB is simple and useful to inspect seg faults.
 *
 * 1) compile your code with the -g flag
 *         e.g. gcc -g main.c -o main
 *
 * 2) run the executable with gdb
 *         e.g. gdb ./main
 *
 * 3) use the 'b' command in gdb to set a breakpoint on a function or line
 *         e.g. b increment_int_pointer
 *              b 7-debug.c:14
 *
 * 4) use the 'p' command in gdb to print out your variables
 *         e.g. p i
 *              p ip
 *              p *ip
 * 4) use the 's' and 'n' commands in gdb to step and skip over parts of your code
 *         s - is step which performs the next instruction diving deeper if its
 *              a function call
 *         n - is next which performs the next instruction but does not dive deeper
 *              if its a function call
 */

#include <stdio.h>

void increment_int_pointer(int* ip)
{
    ++(*ip);
}

void main(int argc, char** argv)
{
    int i = 5;
    int* ip;
    // WILL SEGFAULT WITHOUT THIS LINE
    // ip = &i;
    increment_int_pointer(ip);

    printf("i = %d\n", i);
}
