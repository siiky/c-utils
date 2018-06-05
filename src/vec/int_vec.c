#define VEC_IMPLEMENTATION
#include <stdio.h>

#include "int_vec.h"

bool int_vec_init (struct int_vec * self, size_t capacity)
{
    bool ret = int_with_capacity(self, capacity);

    if (ret) {
        for (size_t i = 1; i <= capacity; i++) {
            int x = rand() % i; /* `Floating point exception` if `i` starts at 0 */
            int_push(self, x);
        }
    }

    return ret;
}

void int_vec_print (struct int_vec * vec)
{
    for (int_iter(vec); int_itering(vec); int_iter_next(vec))
        printf("%d ", int_get_nth(vec, int_iter_idx(vec)));
    putchar('\n');
}
