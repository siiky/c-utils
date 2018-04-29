#define VEC_IMPLEMENTATION
#include <stdio.h>

#include "char_vec.h"

char char_dtor (char elem)
{
    return elem;
}

struct char_vec * char_vec_init (size_t capacity)
{
    struct char_vec * ret = char_new();
    if (ret == NULL) return NULL;
    for (size_t i = 1; i <= capacity; i++)
        char_push(ret, rand() % 256);
    return ret;
}

char char_print (char elem)
{
    putchar(elem);
    putchar(' ');
    return elem;
}

void char_vec_print (struct char_vec * vec)
{
    char_map(vec, char_print);
    putchar('\n');
}
