#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(as_slice, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(as_slice, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(ptr) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    int * pre_ptr = vec->ptr;
    const int * res  = vec_as_slice(vec);

    return QC_BOOL2TRIAL(pre_ptr == res);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec pre_dup = {0};
    if (!qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    vec_as_slice(vec);

    size_t pos_len = vec->length;

    bool ret = pos_len == 0
        || memcmp(pre_dup.ptr, vec->ptr, pos_len * sizeof(int)) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    bool pre_iterating = vec->iterating;
    bool pre_reverse = vec->reverse;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    vec_as_slice(vec);

    bool pos_iterating = vec->iterating;
    bool pos_reverse = vec->reverse;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;

    bool res = true
        && pre_cap       == pos_cap
        && pre_idx       == pos_idx
        && pre_iterating == pos_iterating
        && pre_len       == pos_len
        && pre_reverse   == pos_reverse;

    return QC_BOOL2TRIAL(res);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(ptr);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(as_slice),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(ptr),
        );
