#include "ii_map.h"

#include <stdio.h>

int main (void)
{
    struct ii_map map;

    if (!ii_map_with_size(&map, 10))
        return !0;

    bool succ = true;

#define _(b1, b2) ((b1) = (b1) && (b2))
    _(succ, ii_map_add(&map, 1,  1));
    _(succ, ii_map_add(&map, 11, 2));
    _(succ, ii_map_add(&map, 12, 3));
    _(succ, ii_map_add(&map, 2,  4));
#undef _

    puts((succ) ? "yay" : "noes");

    map = ii_map_free(map);

    return 0;
}
