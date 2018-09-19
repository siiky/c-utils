#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(shrink_to_fit, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(shrink_to_fit, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    bool pre_itering = vec->iterating;
    bool pre_rev = vec->reverse;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    vec_shrink_to_fit(vec);

    bool pos_itering = vec->iterating;
    bool pos_rev = vec->reverse;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;

    bool ret = true
        && pos_cap >= pos_len
        && pre_cap >= pos_cap
        && pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_len == pos_len
        && pre_rev == pos_rev;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    bool res = vec_shrink_to_fit(vec);

    bool ret = !res
        || memcmp(vec->ptr, dup.ptr, dup.length * sizeof(int)) == 0;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(shrink_to_fit),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        );
