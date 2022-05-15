#include <stdio.h>

int main()
{
    //
    //
    //
    // STRINGS
    //
    //
    //


    /*
     * No more basic string type :(
     * in C strings are just thought of as character arrays and are normally
     * typed as const char*
     */
    const char* my_first_name = "Scott";
    printf("My first name is.. %s\n", my_first_name); 

    /*
     * These character arrays are a little weird in C though
     * If you try to construct one yourself or copy one you need to know
     * about the null-terminator character. This is a character that C uses
     * to denote the end of a string
     */
    char my_last_name[6] = {'M', 'u', 'n', 'r', 'o', '\0'};
    printf("My last name is... %s\n", my_last_name);


    //
    //
    //
    // ARRAYS
    //
    //
    //

    /* 
     * In C, arrays are declared like this..
     * type name [size];
     */
    int int_arr[6]; // makes an array of 6 ints

    /*
     * Arrays have no Length property and its advised not to use sizeof
     * on arrays
     *
     * the following does not work:
     * int length = int_arr.length;
     * 
     * neither does this
     *
     * for(int i = 0; i < int_arr.length; ++i)
     * {
     *      // do something with i
     * }
     */
}
