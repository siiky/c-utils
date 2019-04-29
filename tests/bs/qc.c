#include "bs.c"

#include "cmp.c"

/* redefine warning */
#define QC_MKID_PROP
#define QC_MKID_TEST
#define QC_MKTEST_FUNC

QC_MKTEST_ALL(qc_bs_test_all,
        QC_MKID_MOD_ALL(cmp),
        );
