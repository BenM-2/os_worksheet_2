/**
 * @file string.h
 * @author Ben Marples
 * @brief Holds string related functions
 */

#ifndef K_STRING_H
#define K_STRING_H

static inline int strlen(const char *const buf)
{
    int length = 0;
    while (buf[length] != '\0')
    {
        length++;
    }
    return length;
}

static inline int strcmp(const char *string_a, const char *string_b)
{
    int len_string_a = strlen(string_a);
    int len_string_b = strlen(string_b);
    if (len_string_a != len_string_b)
    {
        return 1;
    }

    for (int i = 0; i < len_string_a; i++)
    {
        if (string_a[i] != string_b[i])
        {
            return 1;
        }
    }
    return 0;
}



#endif