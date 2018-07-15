#include <stdbool.h>
#include <theft.h>

#include "vec.h"

#include "pop.h"
#include "push.h"
#include "swap_remove.h"

#include <common.h>

QC_MKTEST_ALL(qc_vec_test_all,
        qc_vec_push_test_all,
        qc_vec_pop_test_all,
        qc_vec_swap_remove_test_all
        );
