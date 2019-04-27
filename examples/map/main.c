#include "map.h"

#include <stdio.h>

#define MAP_SIZE   100003
#define MAP_NELEMS (1 << 23)
#define MAP_CFG_KEY_DATA_TYPE unsigned
#define MAP_CFG_VALUE_DATA_TYPE unsigned

int main (void)
{
    struct map map;

    if (!map_with_size(&map, MAP_SIZE))
        return !0;

    bool succ = true;
    for (unsigned i = 0; i < MAP_NELEMS; i++)
        succ = map_add(&map, i, i) && succ;

    size_t cardinal_before = map_cardinal(&map);

    for (unsigned i = 0; i < MAP_NELEMS; i++)
        if (map_contains(&map, i))
            map_remove(&map, i);
    size_t cardinal_after = map_cardinal(&map);

    map = map_free(map);

    size_t min = map_data_mem_usage(MAP_NELEMS);
    size_t used = map_expected_mem_usage(MAP_SIZE, MAP_NELEMS);

    puts((succ) ? "yay" : "nay");
    printf("cardinal: %zu %zu\n", cardinal_before, cardinal_after);
    printf("minimum %zu (%zuM)\n"
            "used %zu (%zuM)\n"
            "used/min %lf\n",
            min, min/1024/1024,
            used, used/1024/1024,
            (double) used / (double) min);
    return 0;
}
