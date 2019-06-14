#include "map.h"

#include <stdio.h>

#define MAP_SIZE   100003U
#define MAP_NELEMS (1U << 23)
#define MAP_CFG_KEY_DATA_TYPE unsigned
#define MAP_CFG_VALUE_DATA_TYPE unsigned
#define print(x) (printf("%s = %zu\n", #x, (x)))

#include <sys/time.h>

/* from https://github.com/dryman/hash_bench */
void print_timediff (const char * info, struct timeval start, struct timeval end)
{
    long second = end.tv_sec - start.tv_sec;
    if (end.tv_usec < start.tv_usec)
        second--;
    long usec = (end.tv_usec - start.tv_usec)
        + ((end.tv_usec < start.tv_usec) ?
                1000000:
                0);
    printf("%s%ld.%06ld\n", info, second, usec);
}

int main (void)
{
#define gtod() gettimeofday(tv + tvi++, NULL)

    unsigned tvi = 0;
    struct map map = {0};
    struct timeval tv[5] = {0};

    gtod();
    if (!map_with_size(&map, MAP_SIZE))
        return !0;

    gtod();
    for (unsigned i = 0; i < MAP_NELEMS; i++)
        map_add(&map, i, i);

    gtod();
    for (unsigned i = 0; i < MAP_NELEMS; i++)
        map_contains(&map, i);
    //if (map_contains(&map, i))
    //    map_remove(&map, i);

    gtod();
    map = map_free(map);
    gtod();

    size_t min = map_data_mem_usage(MAP_NELEMS);
    size_t used = map_expected_mem_usage(MAP_SIZE, MAP_NELEMS);
    print(sizeof(struct map));
    printf("\n"
            "# of entries:           %u\n"
            "# of arrays:            %u\n"
            "# of entries per array: %u\n"
            "minimum:                %zuB (%zuM)\n"
            "used:                   %zuB (%zuM)\n"
            "used/min:               %lf\n",
            MAP_NELEMS,
            MAP_SIZE,
            MAP_NELEMS / MAP_SIZE,
            min, min >> 20,
            used, used >> 20,
            (double) used / (double) min);

    puts("\nTimes:");
    print_timediff("Initialize:   ", tv[0], tv[1]);
    print_timediff("Insert:       ", tv[1], tv[2]);
    print_timediff("Query&Remove: ", tv[2], tv[3]);
    print_timediff("Free:         ", tv[3], tv[4]);
    print_timediff("Total:        ", tv[0], tv[4]);

    return 0;
}
