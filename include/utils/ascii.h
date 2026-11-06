/**
 * @file ascii.h
 * @author Ben Marples
 * @brief
 */
#ifndef ASCII_H
#define ASCII_H

/**
 * @brief Takes in a digit and returns its char in ascii
 */
static inline void int_to_char(int i, char *str)
{
    int count = 0; 

    if (i == 0)
    {
        str[0] = '0';
        str[1] = '\0'; 
        return;
    }

    if (i < 0)
    {
        str[count++] = '-';
        i = i * -1;
    }

    while (i > 0)
    {
        int last_int = i % 10;
        str[count++] = '0' + last_int;
        i = i / 10;
    }

    str[count--] = '\0';

    int start = 0;
    // reverse in place
    while (start < count )
    {
        char temp = str[start];
        str[start++] = str[count];
        str[count--] = temp;
    }
    
}

#endif