#define VEC_CFG_IMPLEMENTATION
#include "cvec.h"

#include <stdio.h>

char char_dtor (char elem)
{
    return elem;
}

bool cvec_init (struct cvec * self, size_t capacity)
{
    bool ret = cvec_with_capacity(self, capacity);

    if (ret)
        for (size_t i = 1; i <= capacity; i++)
            cvec_push(self, rand() % 256);

    return ret;
}

static void _cvec_print (const char elem)
{
    putchar(elem);
    putchar(' ');
}

void cvec_print (struct cvec * vec)
{
    cvec_foreach(vec, _cvec_print);
    putchar('\n');
}
