#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(filter, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(filter, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static bool _const_true (const int * x)
{
    UNUSED(x);
    return true;
}

static bool _is_even (const int * x)
{
    return ((*x) % 2) == 0;
}

static enum theft_trial_res QC_MKID_PROP(len_const_true) (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    vec_filter(vec, _const_true);
    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res QC_MKID_PROP(len_is_even) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t count = qc_vec_count(vec, _is_even);
    vec_filter(vec, _is_even);
    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pos_len == count);
}

static enum theft_trial_res QC_MKID_PROP(iter) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_filter(vec, _const_true);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(len_const_true);
QC_MKTEST_FUNC(len_is_even);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(filter),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(len_const_true),
        QC_MKID_TEST(len_is_even),
        );
