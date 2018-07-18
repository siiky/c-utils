#include <stdbool.h>
#include <string.h>

#include "vec.h"

#include <common.h>
#include <unused.h>

static enum theft_trial_res qc_vec_as_mut_slice_len_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_len = vec->length;

    vec_as_mut_slice(vec);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res qc_vec_as_mut_slice_ptr_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    int * pre_ptr = vec->ptr;
    int * res  = vec_as_mut_slice(vec);

    return QC_BOOL2TRIAL(pre_ptr == res);
}

static enum theft_trial_res qc_vec_as_mut_slice_content_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec * pre_dup = qc_vec_dup_contents(vec);
    if (pre_dup == NULL)
        return THEFT_TRIAL_SKIP;

    vec_as_mut_slice(vec);

    size_t pos_len = vec->length;

    bool ret = pos_len == 0
        || memcmp(pre_dup->ptr, vec->ptr, pos_len * sizeof(int)) == 0;

    qc_vec_dup_free(pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_as_mut_slice_iter_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_as_mut_slice(vec);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool res = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(res);
}

#define QC_MKTEST_AS_MUT_SLICE(TEST)                 \
    QC_MKTEST(qc_vec_as_mut_slice_ ## TEST ## _test, \
            prop1,                                   \
            qc_vec_as_mut_slice_ ## TEST ## _prop,   \
            &qc_vec_info)

QC_MKTEST_AS_MUT_SLICE(content);
QC_MKTEST_AS_MUT_SLICE(iter);
QC_MKTEST_AS_MUT_SLICE(len);
QC_MKTEST_AS_MUT_SLICE(ptr);

QC_MKTEST_ALL(qc_vec_as_mut_slice_test_all,
        qc_vec_as_mut_slice_content_test,
        qc_vec_as_mut_slice_iter_test,
        qc_vec_as_mut_slice_len_test,
        qc_vec_as_mut_slice_ptr_test,
        );
