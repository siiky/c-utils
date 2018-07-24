#include "vec.h"

static enum theft_trial_res qc_vec_itering_content_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_itering(vec);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity * sizeof(int)) == 0;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_itering_res_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    bool itering = vec->iterating;

    bool res = vec_itering(vec);

    bool ret = itering == res;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_itering_iter_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_itering(vec);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_ITERING(TEST)                 \
    QC_MKTEST(qc_vec_itering_ ## TEST ## _test, \
            prop1,                              \
            qc_vec_itering_ ## TEST ## _prop,   \
            &qc_vec_info)

QC_MKTEST_ITERING(content);
QC_MKTEST_ITERING(iter);
QC_MKTEST_ITERING(res);

QC_MKTEST_ALL(qc_vec_itering_test_all,
        qc_vec_itering_content_test,
        qc_vec_itering_iter_test,
        qc_vec_itering_res_test,
        );
