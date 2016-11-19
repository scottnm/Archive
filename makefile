one: 1-the-familiar.c
	gcc 1-the-familiar.c -o lesson1_thefamiliar

two: 2-functions.c
	gcc 2-functions.c -o lesson2_functions

three: 3-structs.c
	gcc 3-structs.c -o lesson3_structs

four: 4-pointers.c
	gcc 4-pointers.c -o lesson4_pointers

five: 5-misc-c-stuff.c
	gcc 5-misc-c-stuff.c -o lesson5_misc

seven: 7-debug.c
	gcc 7-debug.c -o lesson7_debug

seven-debug: 7-debug.c
	gcc -g 7-debug.c -o lesson7_debug_debuggable

clean:
	rm -rf lesson1_thefamiliar lesson2_functions lesson3_structs lesson4_pointers lesson5_misc lesson7_debug lesson7_debug_debuggable
