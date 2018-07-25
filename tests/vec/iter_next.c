#include "vec.h"

static enum theft_trial_res qc_vec_iter_next_content_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    if (!vec->iterating)
        return THEFT_TRIAL_SKIP;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_iter_next(vec);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_iter_next_idx_len_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    if (!vec->iterating)
        return THEFT_TRIAL_SKIP;

    vec_iter_next(vec);

    return QC_BOOL2TRIAL((vec->length > 0) ?
            vec->idx <  vec->length:
            vec->idx == vec->length);
}

static enum theft_trial_res qc_vec_iter_next_idx_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    if (!vec->iterating)
        return THEFT_TRIAL_SKIP;

    size_t pre_idx = vec->idx;

    vec_iter_next(vec);

    size_t pos_idx = vec->idx;

    bool ret = (vec->reverse) ?
        pos_idx <= pre_idx:
        pos_idx >= pre_idx;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_ITER_NEXT(TEST)                 \
    QC_MKTEST(qc_vec_iter_next_ ## TEST ## _test, \
            prop1,                                \
            qc_vec_iter_next_ ## TEST ## _prop,   \
            &qc_vec_info)

QC_MKTEST_ITER_NEXT(content);
QC_MKTEST_ITER_NEXT(idx);
QC_MKTEST_ITER_NEXT(idx_len);

QC_MKTEST_ALL(qc_vec_iter_next_test_all,
        qc_vec_iter_next_content_test,
        qc_vec_iter_next_idx_len_test,
        qc_vec_iter_next_idx_test,
        );
