#include "vec.h"

bool QC_MKID_MOD_ALL(append)        (void);
bool QC_MKID_MOD_ALL(as_mut_slice)  (void);
bool QC_MKID_MOD_ALL(as_slice)      (void);
bool QC_MKID_MOD_ALL(bsearch)       (void);
bool QC_MKID_MOD_ALL(cap)           (void);
bool QC_MKID_MOD_ALL(elem)          (void);
bool QC_MKID_MOD_ALL(filter)        (void);
bool QC_MKID_MOD_ALL(find)          (void);
bool QC_MKID_MOD_ALL(foreach)       (void);
bool QC_MKID_MOD_ALL(foreach_range) (void);
bool QC_MKID_MOD_ALL(get_nth)       (void);
bool QC_MKID_MOD_ALL(insert)        (void);
bool QC_MKID_MOD_ALL(is_empty)      (void);
bool QC_MKID_MOD_ALL(iter)          (void);
bool QC_MKID_MOD_ALL(iter_end)      (void);
bool QC_MKID_MOD_ALL(iter_next)     (void);
bool QC_MKID_MOD_ALL(iter_rev)      (void);
bool QC_MKID_MOD_ALL(itering)       (void);
bool QC_MKID_MOD_ALL(len)           (void);
bool QC_MKID_MOD_ALL(map)           (void);
bool QC_MKID_MOD_ALL(map_range)     (void);
bool QC_MKID_MOD_ALL(pop)           (void);
bool QC_MKID_MOD_ALL(push)          (void);
bool QC_MKID_MOD_ALL(qsort)         (void);
bool QC_MKID_MOD_ALL(remove)        (void);
bool QC_MKID_MOD_ALL(reserve)       (void);
bool QC_MKID_MOD_ALL(set_len)       (void);
bool QC_MKID_MOD_ALL(set_nth)       (void);
bool QC_MKID_MOD_ALL(shrink_to_fit) (void);
bool QC_MKID_MOD_ALL(split_off)     (void);
bool QC_MKID_MOD_ALL(swap_remove)   (void);
bool QC_MKID_MOD_ALL(truncate)      (void);
bool QC_MKID_MOD_ALL(with_cap)      (void);

QC_MKTEST_ALL(qc_vec_test_all,
        QC_MKID_MOD_ALL(append),
        QC_MKID_MOD_ALL(as_mut_slice),
        QC_MKID_MOD_ALL(as_slice),
        QC_MKID_MOD_ALL(bsearch),
        QC_MKID_MOD_ALL(cap),
        QC_MKID_MOD_ALL(elem),
        QC_MKID_MOD_ALL(filter),
        QC_MKID_MOD_ALL(find),
        QC_MKID_MOD_ALL(foreach),
        QC_MKID_MOD_ALL(foreach_range),
        QC_MKID_MOD_ALL(get_nth),
        QC_MKID_MOD_ALL(insert),
        QC_MKID_MOD_ALL(is_empty),
        QC_MKID_MOD_ALL(iter),
        QC_MKID_MOD_ALL(iter_end),
        QC_MKID_MOD_ALL(iter_next),
        QC_MKID_MOD_ALL(iter_rev),
        QC_MKID_MOD_ALL(itering),
        QC_MKID_MOD_ALL(len),
        QC_MKID_MOD_ALL(map),
        QC_MKID_MOD_ALL(map_range),
        QC_MKID_MOD_ALL(pop),
        QC_MKID_MOD_ALL(push),
        QC_MKID_MOD_ALL(qsort),
        QC_MKID_MOD_ALL(remove),
        QC_MKID_MOD_ALL(reserve),
        QC_MKID_MOD_ALL(set_len),
        QC_MKID_MOD_ALL(set_nth),
        QC_MKID_MOD_ALL(shrink_to_fit),
        QC_MKID_MOD_ALL(split_off),
        QC_MKID_MOD_ALL(swap_remove),
        QC_MKID_MOD_ALL(truncate),
        QC_MKID_MOD_ALL(with_cap),
        );
