#include "map.h"

bool QC_MKID_MOD_ALL(contains) (void);
bool QC_MKID_MOD_ALL(get)      (void);

QC_MKTEST_ALL(qc_map_test_all,
        QC_MKID_MOD_ALL(contains),
        QC_MKID_MOD_ALL(get),
        );
