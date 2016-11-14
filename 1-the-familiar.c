// includes interface for standard input/output operations
#include <stdio.h>

// similar entry point
// Just like "public static void main(String[] args) in Java
void main(int argc, char** argv)
{
    // similar primitives with similar simple operations
    char character = 'a';
    int integer = 10;
    float floating_point = 10.0 * 5;
    double double_floating_point = floating_point + 5.5;

    // similar function calls
    printf("A character:\t%c\n", character);
    printf("An integer:\t%d\n", integer);
    printf("A float:\t%.2f\n", floating_point);
    printf("A double:\t%.2f\n", double_floating_point);

    // familiar control flow
    int i;
    for (i = 0; i < 10; ++i)
    {
        printf("%-2d is... ", i);
        if (i % 2 == 0)
        {
            printf("even\n");
        }
        else
        {
            printf("odd\n");
        }
    }
}
