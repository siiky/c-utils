#include <stdbool.h>
#include <string.h>

#include "pop.h"

#include "vec.h"

#include <common.h>
#include <unused.h>

static enum theft_trial_res qc_vec_pop_len_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = (struct vec *) arg1;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    vec_pop(vec);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == (pos_len + 1));
}

static enum theft_trial_res qc_vec_pop_elem_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = (struct vec *) arg1;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    int pre_elem = vec->ptr[pre_len - 1];

    int elem = vec_pop(vec);

    return QC_BOOL2TRIAL(elem == pre_elem);
}

static enum theft_trial_res qc_vec_pop_content_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = (struct vec *) arg1;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    struct vec * pre_dup = qc_vec_dup_contents(vec);
    if (pre_dup == NULL)
        return THEFT_TRIAL_SKIP;

    vec_pop(vec);

    size_t pos_len = vec->length;

    bool ret = true
        && pre_dup->iterating == vec->iterating
        && pre_dup->reverse   == vec->reverse
        && pre_dup->idx       == vec->idx
        && (pos_len == 0
                || memcmp(pre_dup->ptr, vec->ptr, pos_len * sizeof(int)) == 0);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST(qc_vec_pop_len_test,
        prop1,
        qc_vec_pop_len_prop,
        &qc_vec_info
        );

QC_MKTEST(qc_vec_pop_elem_test,
        prop1,
        qc_vec_pop_elem_prop,
        &qc_vec_info
        );

QC_MKTEST(qc_vec_pop_content_test,
        prop1,
        qc_vec_pop_content_prop,
        &qc_vec_info
        );

QC_MKTEST_ALL(qc_vec_pop_test_all,
        qc_vec_pop_len_test,
        qc_vec_pop_elem_test,
        qc_vec_pop_content_test
        );
