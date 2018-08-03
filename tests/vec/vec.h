#define VEC_CFG_DATA_TYPE int
#include <vec.h>

#ifndef _VEC_DEFS_H
#define _VEC_DEFS_H

#include <stdbool.h>
#include <string.h>

#include <common.h>
#include <unused.h>

extern const struct theft_type_info qc_vec_info;

bool   qc_vec_dup_contents (const struct vec * self, struct vec * dup);
bool   qc_vec_search       (struct vec * self, int elem, size_t * _i);
size_t qc_vec_count        (struct vec * self, bool pred (const int *));
void   qc_vec_dup_free     (struct vec * self);

#define QC_MKID_MOD(FUNC, TEST, TYPE) \
    QC_MKID(vec, FUNC, TEST, TYPE)

#endif /* _VEC_DEFS_H */
