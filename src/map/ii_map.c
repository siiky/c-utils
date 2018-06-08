static unsigned int ii_hash (const int key)
{
    return key;
}

static int ii_cmp (const int a, const int b)
{
    return (a < b) ?
        -1:
        (a > b) ?
        1:
        0;
}

#define MAP_CFG_KEY_CMP ii_cmp
#define MAP_CFG_HASH_FUNC ii_hash
#define MAP_CFG_IMPLEMENTATION
#include "ii_map.h"
