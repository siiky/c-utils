#define VEC_IMPLEMENTATION
#include <stdio.h>

#include "ivec.h"

bool ivec_init (struct ivec * self, size_t capacity)
{
    bool ret = ivec_with_capacity(self, capacity);

    if (ret) {
        for (size_t i = 1; i <= capacity; i++) {
            int x = rand() % i; /* `Floating point exception` if `i` starts at 0 */
            ivec_push(self, x);
        }
    }

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
