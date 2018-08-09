#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(len, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(len, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    bool pre_iter = vec->iterating;
    bool pre_rev = vec->reverse;
    int * pre_ptr = vec->ptr;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    vec_len(vec);

    bool pos_iter = vec->iterating;
    bool pos_rev = vec->reverse;
    int * pos_ptr = vec->ptr;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;

    bool ret = true
        && pre_cap == pos_cap
        && pre_idx == pos_idx
        && pre_iter == pos_iter
        && pre_len == pos_len
        && pre_ptr == pos_ptr
        && pre_rev == pos_rev;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    size_t len = vec->length;

    size_t res_len = vec_len(vec);

    return QC_BOOL2TRIAL(len == res_len);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_len(vec);

    bool ret = memcmp(vec->ptr, dup.ptr, vec->length * sizeof(int)) == 0;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(len),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(res),
        );
