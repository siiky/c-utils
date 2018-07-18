#define VEC_CFG_DATA_TYPE int
#include <vec.h>

#ifndef _VEC_DEFS_H
#define _VEC_DEFS_H

#include <theft.h>

extern const struct theft_type_info qc_vec_info;

bool         qc_vec_search       (struct vec * self, int elem, size_t * _i);
struct vec * qc_vec_dup_contents (struct vec * self);

#endif /* _VEC_DEFS_H */
