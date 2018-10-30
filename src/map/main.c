#include "map.h"

#include <stdio.h>

#define MAP_SIZE   1000
#define MAP_NELEMS 1000000

int main (void)
{
    struct map map;

    if (!map_with_size(&map, MAP_SIZE))
        return !0;

    bool succ = true;

    for (unsigned int i = 0; i < MAP_NELEMS; i++) {
        bool tmp = map_add(&map, i, i);
        succ = succ && tmp;
    }

    for (unsigned int i = 0; i < MAP_NELEMS; i++)
        if (map_contains(&map, i))
            map_remove(&map, i);

    map = map_free(map);

    puts((succ) ? "yay" : "nay");
    return 0;
}
