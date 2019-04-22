static unsigned int hash_func (unsigned key)
{
    return key;
}

static int cmp_func (unsigned a, unsigned b)
{
    return (a < b) ?
        -1:
        (a > b) ?
        1:
        0;
}

#include "tralloc.h"

#define MAP_CFG_MALLOC trmalloc
#define MAP_CFG_CALLOC trcalloc
#define MAP_CFG_REALLOC trrealloc
#define MAP_CFG_FREE trfree
#define MAP_CFG_KEY_CMP cmp_func
#define MAP_CFG_HASH_FUNC hash_func
#define MAP_CFG_IMPLEMENTATION
#include "map.h"

#include <stdio.h>

void map_print_all_elements (const struct map * self)
{
    if (self == NULL)
        return;

    unsigned int size = self->size;

    for (unsigned int l = 0; l < size; l++) {
        unsigned int len = self->table[l].length;

#define _(F) (self->table[l].entries[i].F)
        for (unsigned int i = 0; i < len; i++)
            printf("L:%u\tI:%u\tH:%u\tK:%u\tV:%u\n",
                    l, i, _(hash), _(key), _(value));
#undef _
    }
}
