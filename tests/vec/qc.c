#include "vec.c"

static int _map_id (int elem)
{
    return elem;
}

static int _map_double (int elem)
{
    return elem * 2;
}

static size_t what = 0;
static void _do_nothing (const int x)
{
    UNUSED(x);
    what++;
}

#include "append.c"
#include "as_mut_slice.c"
#include "as_slice.c"
#include "cap.c"
#include "elem.c"
#include "filter.c"
#include "find.c"
#include "foreach.c"
#include "foreach_range.c"
#include "free.c"
#include "free_range.c"
#include "from_raw_parts.c"
#include "get_nth.c"
#include "insert.c"
#include "is_empty.c"
#include "iter.c"
#include "iter_end.c"
#include "iter_idx.c"
#include "iter_next.c"
#include "iter_rev.c"
#include "itering.c"
#include "len.c"
#include "map.c"
#include "map_range.c"
#include "pop.c"
#include "push.c"
#include "qsort.c"
#include "remove.c"
#include "reserve.c"
#include "set_len.c"
#include "set_nth.c"
#include "shrink_to_fit.c"
#include "split_off.c"
#include "swap_remove.c"
#include "truncate.c"
#include "with_cap.c"

/* redefine warning */
#define QC_MKID_PROP
#define QC_MKID_TEST
#define QC_MKTEST_FUNC

QC_MKTEST_ALL(qc_vec_test_all,
        QC_MKID_MOD_ALL(append),
        QC_MKID_MOD_ALL(as_mut_slice),
        QC_MKID_MOD_ALL(as_slice),
        QC_MKID_MOD_ALL(cap),
        QC_MKID_MOD_ALL(elem),
        QC_MKID_MOD_ALL(filter),
        QC_MKID_MOD_ALL(find),
        QC_MKID_MOD_ALL(foreach),
        QC_MKID_MOD_ALL(foreach_range),
        QC_MKID_MOD_ALL(free),
        QC_MKID_MOD_ALL(free_range),
        QC_MKID_MOD_ALL(from_raw_parts),
        QC_MKID_MOD_ALL(get_nth),
        QC_MKID_MOD_ALL(insert),
        QC_MKID_MOD_ALL(is_empty),
        QC_MKID_MOD_ALL(iter),
        QC_MKID_MOD_ALL(iter_end),
        QC_MKID_MOD_ALL(iter_idx),
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
