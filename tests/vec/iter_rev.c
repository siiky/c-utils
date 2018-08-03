#include "vec.h"

static enum theft_trial_res qc_vec_iter_rev_res_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    struct vec * vec = arg1;
    bool rev = * (bool *) arg2;

    bool itering = vec->iterating;

    bool res = vec_iter_rev(vec, rev);

    bool ret = (itering) ?
        !res:
        res;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_iter_rev_content_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    struct vec * vec = arg1;
    bool rev = * (bool *) arg2;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_iter_rev(vec, rev);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_iter_rev_meta_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    struct vec * vec = arg1;
    bool rev = * (bool *) arg2;

    bool pre_itering = vec->iterating;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    vec_iter_rev(vec, rev);

    bool pos_itering = vec->iterating;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;

    bool ret = true
        && pre_cap == pos_cap
        && pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_len == pos_len;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_ITER_REV(TEST)                 \
    QC_MKTEST(qc_vec_iter_rev_ ## TEST ## _test, \
            prop2,                               \
            qc_vec_iter_rev_ ## TEST ## _prop,   \
            &qc_vec_info,                        \
            &qc_bool_info)

QC_MKTEST_ITER_REV(content);
QC_MKTEST_ITER_REV(meta);
QC_MKTEST_ITER_REV(res);

QC_MKTEST_ALL(qc_vec_iter_rev_test_all,
        qc_vec_iter_rev_content_test,
        qc_vec_iter_rev_meta_test,
        qc_vec_iter_rev_res_test,
        );
