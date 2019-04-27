#define VEC_CFG_IMPLEMENTATION
#include "ivec.h"

#include <stdio.h>

bool ivec_init (struct ivec * self, size_t cap)
{
    bool ret = ivec_with_cap(self, cap);

    if (ret)
        for (size_t i = 1; i <= cap; i++)
            ivec_push(self, rand() % 100000000);

    return ret;
}

static void _int_print (const int e)
{
    printf("%d ", e);
}

void ivec_print (struct ivec * vec)
{
    ivec_foreach(vec, _int_print);
    putchar('\n');
}
