#include <stdbool.h>
#include <string.h>

#include <common.h>
#include <unused.h>

#include "vec.h"

static bool qc_vec_filter_const_true_pred (const int * x)
{
    UNUSED(x);
    return true;
}

static bool qc_vec_filter_is_even_pred (const int * x)
{
    return ((*x) % 2) == 0;
}

static enum theft_trial_res qc_vec_filter_len_const_true_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    vec_filter(vec, qc_vec_filter_const_true_pred);
    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res qc_vec_filter_len_is_even_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t count = qc_vec_count(vec, qc_vec_filter_is_even_pred);
    vec_filter(vec, qc_vec_filter_is_even_pred);
    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pos_len == count);
}

static enum theft_trial_res qc_vec_filter_iter_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_filter(vec, qc_vec_filter_const_true_pred);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_FILTER(TEST)                 \
    QC_MKTEST(qc_vec_filter_ ## TEST ## _test, \
            prop1,                             \
            qc_vec_filter_ ## TEST ## _prop,   \
            &qc_vec_info)

QC_MKTEST_FILTER(iter);
QC_MKTEST_FILTER(len_const_true);
QC_MKTEST_FILTER(len_is_even);

QC_MKTEST_ALL(qc_vec_filter_test_all,
        qc_vec_filter_iter_test,
        qc_vec_filter_len_const_true_test,
        qc_vec_filter_len_is_even_test,
        );
