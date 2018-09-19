#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(split_off, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(split_off, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info)

/* correct input tests */
static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, at);

    {
        size_t len = vec->length;
        if (at >= len) {
            at = at % (len + 1);
            QC_ARG2VAL(2, size_t) = at;
        }
    }

    struct vec other = {0};

    bool pre_iter = vec->iterating;
    bool pre_rev = vec->reverse;
    size_t pre_len = vec->length;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;

    vec_split_off(vec, &other, at);

    bool pos_iter = vec->iterating;
    bool pos_rev = vec->reverse;
    size_t pos_len = vec->length;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;

    size_t other_len = other.length;

    bool ret = true
        && pre_cap >= pos_cap
        && pre_idx == pos_idx
        && pre_iter == pos_iter
        && pre_len == pos_len + other_len
        && pre_rev == pos_rev;

    qc_vec_dup_free(&other);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(vec_content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, at);

    {
        size_t len = vec->length;
        if (at >= len) {
            at = at % (len + 1);
            QC_ARG2VAL(2, size_t) = at;
        }
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    struct vec other = {0};

    bool res = vec_split_off(vec, &other, at);

    bool ret = !res
        || memcmp(vec->ptr, dup.ptr, vec->length * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);
    qc_vec_dup_free(&other);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(other_content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, at);

    {
        size_t len = vec->length;
        if (at >= len) {
            at = at % (len + 1);
            QC_ARG2VAL(2, size_t) = at;
        }
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    struct vec other = {0};

    bool res = vec_split_off(vec, &other, at);

    bool ret = !res
        || memcmp(other.ptr, dup.ptr + vec->length, other.length * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);
    qc_vec_dup_free(&other);

    return QC_BOOL2TRIAL(ret);
}

/* OOB tests */
static enum theft_trial_res QC_MKID_PROP(oob_meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, at);

    {
        size_t len = vec->length;
        if (at < len)
            return QC_MKID_PROP(meta)(t, arg1, arg2);
    }

    struct vec other = {0};

    bool pre_iter = vec->iterating;
    bool pre_rev = vec->reverse;
    size_t pre_len = vec->length;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;

    vec_split_off(vec, &other, at);

    bool pos_iter = vec->iterating;
    bool pos_rev = vec->reverse;
    size_t pos_len = vec->length;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;

    size_t other_len = other.length;

    bool ret = true
        && pre_cap >= pos_cap
        && pre_idx == pos_idx
        && pre_iter == pos_iter
        && pre_len == pos_len + other_len
        && pre_rev == pos_rev;

    qc_vec_dup_free(&other);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(oob_vec_content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, at);

    {
        size_t len = vec->length;
        if (at < len)
            return QC_MKID_PROP(vec_content)(t, arg1, arg2);
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    struct vec other = {0};

    bool res = vec_split_off(vec, &other, at);

    bool ret = !res
        || memcmp(vec->ptr, dup.ptr, vec->length * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);
    qc_vec_dup_free(&other);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(oob_other_content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, at);

    {
        size_t len = vec->length;
        if (at < len)
            return QC_MKID_PROP(other_content)(t, arg1, arg2);
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    struct vec other = {0};

    bool res = vec_split_off(vec, &other, at);

    bool ret = !res
        || memcmp(other.ptr, dup.ptr + vec->length, other.length * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);
    qc_vec_dup_free(&other);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(vec_content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(oob_vec_content);
QC_MKTEST_FUNC(oob_meta);
QC_MKTEST_FUNC(oob_other_content);
QC_MKTEST_FUNC(other_content);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(split_off),
        QC_MKID_TEST(vec_content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(oob_vec_content),
        QC_MKID_TEST(oob_meta),
        QC_MKID_TEST(oob_other_content),
        QC_MKID_TEST(other_content),
        );
