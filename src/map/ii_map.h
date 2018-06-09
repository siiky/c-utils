#ifndef _II_MAP_H
# define _II_MAP_H

#define MAP_CFG_MAP ii_map
#define MAP_CFG_KEY_DATA_TYPE unsigned int
#define MAP_CFG_VALUE_DATA_TYPE unsigned int
#include <map.h>

void ii_map_print_all_elements (const struct ii_map * self);

#endif /* _II_MAP_H */
