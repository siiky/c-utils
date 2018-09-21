#include "ii_map.h"

#include <stdio.h>

#define MAP_SIZE   1000
#define MAP_NELEMS 1000000

int main (void)
{
    struct ii_map map;

    if (!ii_map_with_size(&map, MAP_SIZE))
        return !0;

    bool succ = true;

    for (unsigned int i = 0; i < MAP_NELEMS; i++) {
        bool tmp = ii_map_add(&map, i, i);
        succ = succ && tmp;
    }

    for (unsigned int i = 0; i < MAP_NELEMS; i++)
        if (ii_map_contains(&map, i))
            ii_map_remove(&map, i);

    map = ii_map_free(map);

    puts((succ) ? "yay" : "nay");
    return 0;
}
