#ifndef __2_FUNCTIONS_H__
#define __2_FUNCTIONS_H__

char to_uppercase(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c + ('A' - 'a');
    }

    return c;
}

#endif // __2_FUNCTIONS_H__
