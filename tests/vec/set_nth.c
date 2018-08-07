#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(set_nth, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(set_nth, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop3,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info,      \
            &qc_int_info)

/* Correct input tests */
static enum theft_trial_res QC_MKID_PROP(left_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, nth);
    QC_ARG2VAR(3, int, elem);

    size_t len = vec->length;
    if (nth >= len) {
        nth = nth % (len + 1);
        QC_ARG2VAL(2, size_t) = nth;
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    bool res = vec_set_nth(vec, nth, elem);

    bool ret = !res
        || nth == 0
        || memcmp(vec->ptr, dup.ptr, nth * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(right_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, nth);
    QC_ARG2VAR(3, int, elem);

    size_t len = vec->length;
    if (nth >= len) {
        nth = nth % (len + 1);
        QC_ARG2VAL(2, size_t) = nth;
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    bool res = vec_set_nth(vec, nth, elem);

    size_t offset = nth + 1;
    size_t nelems = len - nth - 1;

    bool ret = !res
        || nth == (len - 1)
        || memcmp(vec->ptr + offset, dup.ptr + offset, nelems * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, nth);
    QC_ARG2VAR(3, int, elem);

    size_t len = vec->length;
    if (nth >= len) {
        nth = nth % (len + 1);
        QC_ARG2VAL(2, size_t) = nth;
    }

    bool pre_itering = vec->iterating;
    size_t pre_idx = vec->idx;
    size_t pre_cap = vec->capacity;
    size_t pre_len = vec->length;

    bool res = vec_set_nth(vec, nth, elem);

    bool pos_itering = vec->iterating;
    size_t pos_idx = vec->idx;
    size_t pos_cap = vec->capacity;
    size_t pos_len = vec->length;

    bool ret = !res
        || (pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_cap == pos_cap
        && pre_len == pos_len);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(nth) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, nth);
    QC_ARG2VAR(3, int, elem);

    size_t len = vec->length;
    if (nth >= len) {
        nth = nth % (len + 1);
        QC_ARG2VAL(2, size_t) = nth;
    }

    bool res = vec_set_nth(vec, nth, elem);

    bool ret = !res || vec->ptr[nth] == elem;

    return QC_BOOL2TRIAL(ret);
}

/* OOB tests */
static enum theft_trial_res QC_MKID_PROP(oob_left_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, nth);
    QC_ARG2VAR(3, int, elem);

    size_t len = vec->length;
    if (nth < len)
        return QC_MKID_PROP(left_content)(t, arg1, arg2, arg3);

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    bool res = vec_set_nth(vec, nth, elem);

    bool ret = !res
        || memcmp(vec->ptr, dup.ptr, len * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(oob_right_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, nth);
    QC_ARG2VAR(3, int, elem);

    size_t len = vec->length;
    if (nth < len)
        return QC_MKID_PROP(right_content)(t, arg1, arg2, arg3);

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    bool res = vec_set_nth(vec, nth, elem);

    bool ret = !res
        && memcmp(vec->ptr, dup.ptr, len * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(oob_meta) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, nth);
    QC_ARG2VAR(3, int, elem);

    size_t len = vec->length;
    if (nth < len)
        return QC_MKID_PROP(meta)(t, arg1, arg2, arg3);

    bool pre_itering = vec->iterating;
    size_t pre_idx = vec->idx;
    size_t pre_cap = vec->capacity;
    size_t pre_len = vec->length;

    bool res = vec_set_nth(vec, nth, elem);

    bool pos_itering = vec->iterating;
    size_t pos_idx = vec->idx;
    size_t pos_cap = vec->capacity;
    size_t pos_len = vec->length;

    bool ret = !res
        && pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_cap == pos_cap
        && pre_len == pos_len;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(oob_nth) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, nth);
    QC_ARG2VAR(3, int, elem);

    size_t len = vec->length;
    if (nth < len)
        return QC_MKID_PROP(nth)(t, arg1, arg2, arg3);

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    bool res = vec_set_nth(vec, nth, elem);

    bool ret = !res
        && memcmp(vec->ptr, dup.ptr, len * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(left_content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(nth);
QC_MKTEST_FUNC(oob_left_content);
QC_MKTEST_FUNC(oob_meta);
QC_MKTEST_FUNC(oob_nth);
QC_MKTEST_FUNC(oob_right_content);
QC_MKTEST_FUNC(right_content);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(set_nth),
        QC_MKID_TEST(left_content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(nth),
        QC_MKID_TEST(oob_left_content),
        QC_MKID_TEST(oob_meta),
        QC_MKID_TEST(oob_nth),
        QC_MKID_TEST(oob_right_content),
        QC_MKID_TEST(right_content),
        );
