#ifndef CHECK_ERR
#include <cstdlib>
#define CHECK_ERR(call, success_val, err_name) \
    do\
    {\
        int __x = (call);\
        if (__x != success_val)\
        {\
            printf(err_name" failed %d\n", __x);\
            exit(1);\
        }\
    } while(0)

#endif // CHECK_ERR
