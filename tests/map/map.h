#ifndef _MAP_DEFS_H
#define _MAP_DEFS_H

#include <stdbool.h>
#include <string.h>

#include <common.h>
#include <unused.h>

extern const struct theft_type_info qc_map_info;

int          qc_map_int_cmp  (const int k1, const int k2);
unsigned int qc_map_int_hash (const int k);

#define QC_MKID_MOD_TEST(FUNC, TEST) \
    QC_MKID(map, FUNC, TEST, test)

#define QC_MKID_MOD_PROP(FUNC, TEST) \
    QC_MKID(map, FUNC, TEST, prop)

#define QC_MKID_MOD_ALL(FUNC) \
    QC_MKID_ALL(map, FUNC)

#endif /* _MAP_DEFS_H */

#define MAP_CFG_KEY_DATA_TYPE int
#define MAP_CFG_VALUE_DATA_TYPE int
#define MAP_CFG_KEY_CMP qc_map_int_cmp
#define MAP_CFG_HASH_FUNC qc_map_int_hash
#include <map.h>
