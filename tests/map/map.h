#ifndef _MAP_DEFS_H
#define _MAP_DEFS_H

#include <stdbool.h>
#include <string.h>

#include <common.h>
#include <utils/unused.h>

struct map;

bool     qc_map_contains      (const struct map * map, int key);
int      qc_map_get           (const struct map * map, int k);
int      qc_map_random_in     (struct theft * t, const struct map * map);
int      qc_map_random_not_in (const struct map * map, int k);
unsigned qc_map_cardinal      (const struct map * map);

extern const struct theft_type_info qc_map_info;

#define QC_MKID_MOD_TEST(FUNC, TEST) \
    QC_MKID(map, FUNC, TEST, test)

#define QC_MKID_MOD_PROP(FUNC, TEST) \
    QC_MKID(map, FUNC, TEST, prop)

#define QC_MKID_MOD_ALL(FUNC) \
    QC_MKID_ALL(map, FUNC)

#endif /* _MAP_DEFS_H */

#define MAP_CFG_KEY_DATA_TYPE int
#define MAP_CFG_VALUE_DATA_TYPE int
#include <utils/map.h>
