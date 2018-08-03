#include "vec.h"

static enum theft_trial_res qc_vec_swap_remove_len_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    vec_swap_remove(vec, idx);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == (pos_len + 1));
}

static enum theft_trial_res qc_vec_swap_remove_elem_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    int pre_elem = vec->ptr[idx];

    int elem = vec_swap_remove(vec, idx);

    return QC_BOOL2TRIAL(pre_elem == elem);
}

static enum theft_trial_res qc_vec_swap_remove_last_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    int pre_last = vec->ptr[pre_len - 1];

    vec_swap_remove(vec, idx);

    size_t pos_len = vec->length;
    int pos_elem = (idx < pos_len) ?
        vec->ptr[idx]:
        0;

    return QC_BOOL2TRIAL(idx >= pos_len || pre_last == pos_elem);
}

static enum theft_trial_res qc_vec_swap_remove_left_content_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;

    struct vec pre_dup = {0};
    size_t pre_len = vec->length;
    if (pre_len == 0 || !qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    vec_swap_remove(vec, idx);

    void * pre = pre_dup.ptr;
    void * pos = vec->ptr;
    size_t nbytes = (idx - 1) * sizeof(int);

    bool res = idx == 0
        || memcmp(pre, pos, nbytes) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res qc_vec_swap_remove_right_content_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;

    struct vec pre_dup = {0};
    size_t pre_len = vec->length;
    if (pre_len == 0 || !qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        * (size_t *) arg2 = idx;
    }

    vec_swap_remove(vec, idx);
    size_t pos_len = vec->length;

    void * pre = pre_dup.ptr + idx + 1;
    void * pos = vec->ptr + idx + 1;
    size_t nbytes = (pos_len - idx - 1) * sizeof(int);

    bool res = idx >= pos_len
        || memcmp(pre, pos, nbytes) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res qc_vec_swap_remove_iter_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;

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

    vec_swap_remove(vec, idx);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool res = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(res);
}

#define QC_MKID_FUNC(TEST, TYPE) \
    QC_MKID_MOD(swap_remove, TEST, TYPE)

#define QC_MKID_TEST(TEST) \
    QC_MKID_FUNC(TEST, test)

#define QC_MKID_PROP(TEST) \
    QC_MKID_FUNC(TEST, prop)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info)

QC_MKTEST_FUNC(elem);
QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(last);
QC_MKTEST_FUNC(left_content);
QC_MKTEST_FUNC(len);
QC_MKTEST_FUNC(right_content);

QC_MKTEST_ALL(qc_vec_swap_remove_test_all,
        QC_MKID_TEST(elem),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(last),
        QC_MKID_TEST(left_content),
        QC_MKID_TEST(len),
        QC_MKID_TEST(right_content),
        );
