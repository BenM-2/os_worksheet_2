
#ifndef KMEM_H
#define KMEM_H

#include "type.h"

static inline void *memset(void *mem, unsigned char value, usize count)
{
    unsigned char * area = (unsigned char *)mem; 
    for (usize i = 0; i < count; i++)
    {
        area[i] = value;
    }

    return (void *)area;
}

#endif 