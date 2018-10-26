#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(reserve, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(reserve, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info)

#define _QC_PRE()                      \
    do {                               \
        total = total % 1024;          \
        QC_ARG2VAL(2, size_t) = total; \
    } while (0)

static enum theft_trial_res QC_MKID_PROP(capacity) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, total);

    _QC_PRE();

    bool res = vec_reserve(vec, total);

    bool ret = !res || vec->capacity >= total;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(cap_len) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, total);

    _QC_PRE();

    vec_reserve(vec, total);

    bool ret = vec->capacity >= vec->length;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, total);

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    _QC_PRE();

    vec_reserve(vec, total);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, total);

    bool pre_itering = vec->iterating;
    bool pre_rev = vec->reverse;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    _QC_PRE();

    vec_reserve(vec, total);

    bool pos_itering = vec->iterating;
    bool pos_rev = vec->reverse;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;

    bool ret = true
        && pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_len == pos_len
        && pre_rev == pos_rev;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(cap_len);
QC_MKTEST_FUNC(capacity);
QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(reserve),
        QC_MKID_TEST(cap_len),
        QC_MKID_TEST(capacity),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        );
