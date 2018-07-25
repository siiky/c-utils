#include "vec.h"

static enum theft_trial_res qc_vec_pop_len_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

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

    struct vec * vec = arg1;

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

    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    struct vec pre_dup = {0};
    if (pre_len == 0 || !qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;


    vec_pop(vec);

    size_t pos_len = vec->length;

    bool ret = pos_len == 0
        || memcmp(pre_dup.ptr, vec->ptr, pos_len * sizeof(int)) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_pop_iter_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_pop(vec);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool res = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(res);
}

#define QC_MKTEST_POP(TEST)                 \
    QC_MKTEST(qc_vec_pop_ ## TEST ## _test, \
            prop1,                          \
            qc_vec_pop_ ## TEST ## _prop,   \
            &qc_vec_info)

QC_MKTEST_POP(content);
QC_MKTEST_POP(elem);
QC_MKTEST_POP(iter);
QC_MKTEST_POP(len);

QC_MKTEST_ALL(qc_vec_pop_test_all,
        qc_vec_pop_content_test,
        qc_vec_pop_elem_test,
        qc_vec_pop_iter_test,
        qc_vec_pop_len_test,
        );
