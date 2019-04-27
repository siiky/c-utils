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

size_t map_data_mem_usage (unsigned nelems)
{
    return nelems * (sizeof(unsigned) + sizeof(unsigned));
}

size_t map_expected_mem_usage (unsigned size, unsigned nelems)
{
    struct map map;
    return map_data_mem_usage(nelems)
        + sizeof(struct map)
        + size * sizeof(*map.table)
        + nelems * sizeof(unsigned);
}
