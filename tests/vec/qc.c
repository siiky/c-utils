#include <stdbool.h>
#include <theft.h>

#include "vec.h"

#include "push.h"
#include "pop.h"

#include <common.h>

QC_MKTEST_ALL(qc_vec_test_all,
        qc_vec_push_test_all,
        qc_vec_pop_test_all
        );
