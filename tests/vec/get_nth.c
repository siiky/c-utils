#include "vec.h"

static enum theft_trial_res qc_vec_get_nth_elem_prop (struct theft * t, void * arg1, void * arg2)
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

    int elem = vec_get_nth(vec, idx);

    return QC_BOOL2TRIAL(pre_elem == elem);
}

static enum theft_trial_res qc_vec_get_nth_content_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;

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

static enum theft_trial_res qc_vec_get_nth_iter_prop (struct theft * t, void * arg1, void * arg2)
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

#define QC_MKTEST_GET_NTH(TEST)                 \
    QC_MKTEST(qc_vec_get_nth_ ## TEST ## _test, \
            prop2,                              \
            qc_vec_get_nth_ ## TEST ## _prop,   \
            &qc_vec_info,                       \
            &qc_size_t_info)

QC_MKTEST_GET_NTH(content);
QC_MKTEST_GET_NTH(elem);
QC_MKTEST_GET_NTH(iter);

QC_MKTEST_ALL(qc_vec_get_nth_test_all,
        qc_vec_get_nth_content_test,
        qc_vec_get_nth_elem_test,
        qc_vec_get_nth_iter_test,
        );
