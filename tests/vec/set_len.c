#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(set_len, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(set_len, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info)

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t len = * (size_t *) arg2;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_set_len(vec, len);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t len = * (size_t *) arg2;

    bool pre_itering = vec->iterating;
    size_t pre_idx = vec->idx;
    size_t pre_cap = vec->capacity;

    vec_set_len(vec, len);

    bool pos_itering = vec->iterating;
    size_t pos_idx = vec->idx;
    size_t pos_cap = vec->capacity;

    bool ret = true
        && pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_cap == pos_cap;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(len) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t len = * (size_t *) arg2;

    vec_set_len(vec, len);

    return QC_BOOL2TRIAL(vec->length == len);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(len);
QC_MKTEST_FUNC(meta);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(set_len),
        QC_MKID_TEST(content),
        QC_MKID_TEST(len),
        QC_MKID_TEST(meta),
        );
