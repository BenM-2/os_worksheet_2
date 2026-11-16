
#ifndef KMEM_H
#define KMEM_H

#include "type.h"

static inline void *memset(void *mem, unsigned char value, usize count)
{
    unsigned char *area = (unsigned char *)mem;
    for (usize i = 0; i < count; i++)
    {
        area[i] = value;
    }

    return (void *)area;
}

static inline void *memcpy(void *dest, const void *src, usize n)
{
    if (dest == NULL || src == NULL)
    {
        return NULL;
    }

    // cast to char for byte manip
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (usize i = 0; i < n; i++)
    {
        d[i] = s[i];
    }

    return dest;
}

#endif