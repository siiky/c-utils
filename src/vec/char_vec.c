#define VEC_IMPLEMENTATION
#include <stdio.h>

#include "char_vec.h"

struct char_vec char_vec_init (size_t capacity)
{
    struct char_vec ret = char_with_capacity(capacity);
    for (size_t i = 1; i <= ret.capacity; i++) {
#define R(I, X) ((rand() % (I)) + (X))
        char_push(&ret, R(i + capacity, 'a'));
        char_push(&ret, R(i + capacity, 'A'));
        char_push(&ret, R(i + capacity, '0'));
#undef R
    }
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
