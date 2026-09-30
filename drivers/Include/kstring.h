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
    while (buf[length] != '\0') {
        length++;
    }
    return length;
}
#endif