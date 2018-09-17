#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(get_nth, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(get_nth, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info)

static enum theft_trial_res QC_MKID_PROP(elem) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, idx);

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    int pre_elem = vec->ptr[idx];

    int elem = vec_get_nth(vec, idx);

    return QC_BOOL2TRIAL(pre_elem == elem);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, idx);

    size_t pre_len = vec->length;
    struct vec dup = {0};
    if (pre_len == 0 || !qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    vec_get_nth(vec, idx);

    bool res = memcmp(vec->ptr, dup.ptr, pre_len * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res QC_MKID_PROP(iter) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, idx);

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_get_nth(vec, idx);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool res = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(res);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(elem);
QC_MKTEST_FUNC(iter);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(get_nth),
        QC_MKID_TEST(content),
        QC_MKID_TEST(elem),
        QC_MKID_TEST(iter),
        );
