#define VEC_IMPLEMENTATION
#include <stdio.h>

#include "char_vec.h"

char char_dtor (char elem)
{
    return elem;
}

bool char_vec_init (struct char_vec * self, size_t capacity)
{
    bool ret = char_with_capacity(self, capacity);

    if (ret)
        for (size_t i = 1; i <= capacity; i++)
            char_push(self, rand() % 256);

    return ret;
}

static void _char_print (const char elem)
{
    putchar(elem);
    putchar(' ');
}

void char_vec_print (struct char_vec * vec)
{
    char_foreach(vec, _char_print);
    putchar('\n');
}
