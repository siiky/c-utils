#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(with_cap, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(with_cap, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info)

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, capacity);

    qc_vec_dup_free(vec);

    bool res = vec_with_cap(vec, capacity);

    bool ret = !res
        || (vec->ptr != NULL
                && vec->capacity == capacity
                && vec->idx == 0
                && vec->iterating == 0
                && vec->length == 0
                && vec->reverse == 0);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(meta);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(with_cap),
        QC_MKID_TEST(meta),
        );
