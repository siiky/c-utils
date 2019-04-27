#ifndef _II_MAP_H
# define _II_MAP_H

#define MAP_CFG_MAP map
#define MAP_CFG_KEY_DATA_TYPE unsigned
#define MAP_CFG_VALUE_DATA_TYPE unsigned
#include <utils/map.h>

#include <stddef.h>

size_t map_data_mem_usage     (unsigned nelems);
size_t map_expected_mem_usage (unsigned size, unsigned nelems);
void   map_print_all_elements (const struct map * self);

#endif /* _II_MAP_H */
