#include <stdio.h>

void increment_int_pointer(int* ip)
{
    ++(*ip);
}

void main(int argc, char** argv)
{
    int i = 5;
    int* ip;
    // WILL SEGFAULT WITHOUT THIS LINE ip = &i;
    increment_int_pointer(ip);

    printf("i = %d\n", i);
}
