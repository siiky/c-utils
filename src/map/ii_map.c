static unsigned int ii_hash (const unsigned int key)
{
    return key;
}

static int ii_cmp (const unsigned int a, const unsigned int b)
{
    return (a < b) ?
        -1:
        (a > b) ?
        1:
        0;
}

#define MAP_CFG_KEY_CMP ii_cmp
#define MAP_CFG_HASH_FUNC ii_hash
#define MAP_CFG_IMPLEMENTATION
#include "ii_map.h"

#include <stdio.h>

void ii_map_print_all_elements (const struct ii_map * self)
{
    if (self == NULL)
        return;

    unsigned int size = self->size;

    for (unsigned int l = 0; l < size; l++) {
        unsigned int len = self->map[l].length;

#define _(F) (self->map[l].entries[i].F)
        for (unsigned int i = 0; i < len; i++)
            printf("L:%u\tI:%u\tH:%u\tK:%u\tV:%u\n",
                    l, i, _(hash), _(key), _(value));
#undef _
    }
}
