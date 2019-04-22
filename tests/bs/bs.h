#include <utils/bs.h>

#ifndef _BS_DEFS_H
#define _BS_DEFS_H

#include <common.h>
#include <utils/unused.h>

#include <string.h>

extern const struct theft_type_info qc_bs_info;
extern const size_t bs_byte_size;

#define bs_nbits2idx(nbits) (nbits / 8)
#define bs_nbits2len(nbits) ((nbits / 8) + ((nbits % 8 != 0) ? 1 : 0))

#define QC_MKID_MOD_TEST(FUNC, TEST) \
    QC_MKID(bs, FUNC, TEST, test)

#define QC_MKID_MOD_PROP(FUNC, TEST) \
    QC_MKID(bs, FUNC, TEST, prop)

#define QC_MKID_MOD_ALL(FUNC) \
    QC_MKID_ALL(bs, FUNC)

#endif /* _BS_DEFS_H */
