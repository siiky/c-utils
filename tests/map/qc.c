#include "map.c"

#include "contains.c"
#include "get.c"

/* redefine warning */
#define QC_MKID_PROP
#define QC_MKID_TEST
#define QC_MKTEST_FUNC

QC_MKTEST_ALL(qc_map_test_all,
        QC_MKID_MOD_ALL(contains),
        QC_MKID_MOD_ALL(get),
        );
