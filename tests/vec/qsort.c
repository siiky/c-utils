#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(qsort, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(qsort, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(is_sorted) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    vec_qsort(vec, qc_int_compar);

    return QC_BOOL2TRIAL(qc_vec_is_sorted(vec));
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    bool pre_itering = vec->iterating;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    vec_qsort(vec, qc_int_compar);

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

QC_MKTEST_FUNC(is_sorted);
QC_MKTEST_FUNC(meta);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(qsort),
        QC_MKID_TEST(is_sorted),
        QC_MKID_TEST(meta),
        );
