#include "ii_map.h"

#include <stdio.h>

int main (void)
{
    struct ii_map map;

    if (!ii_map_with_size(&map, 1000))
        return !0;

    bool succ = true;

#define _(b1, b2) ((b1) = (b1) && (b2))
    for (int i = 0; i < 1000000; i++)
        _(succ, ii_map_add(&map, i, i));
#undef _

    //ii_map_print_all_elements(&map);

    puts((succ) ? "yay" : "noes");

    map = ii_map_free(map);

    return 0;
}
