#include <stdbool.h>
#include <string.h>

#include "swap_remove.h"

#include <common.h>
#include <unused.h>

#include "vec.h"

static enum theft_trial_res qc_vec_swap_remove_len_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    vec_swap_remove(vec, idx);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == (pos_len + 1));
}

QC_MKTEST(qc_vec_swap_remove_len_test,
        prop2,
        qc_vec_swap_remove_len_prop,
        &qc_vec_info,
        &qc_size_t_info
        );

QC_MKTEST_ALL(qc_vec_swap_remove_test_all,
        qc_vec_swap_remove_len_test
        );
