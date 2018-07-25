#include "vec.h"

static enum theft_trial_res qc_vec_find_len_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    size_t pre_len = vec->length;

    vec_find(vec, elem);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res qc_vec_find_elem_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    size_t i = 0;
    size_t it = vec_find(vec, elem);
    qc_vec_search(vec, elem, &i);

    return QC_BOOL2TRIAL(i == it);
}

static enum theft_trial_res qc_vec_find_content_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    struct vec pre_dup = {0};
    if (!qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    size_t pre_len = vec->length;

    vec_find(vec, elem);

    size_t nbytes = pre_len * sizeof(int);
    bool ret = memcmp(pre_dup.ptr, vec->ptr, nbytes) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_find_iter_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_find(vec, elem);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_FIND(TEST)                 \
    QC_MKTEST(qc_vec_find_ ## TEST ## _test, \
            prop2,                           \
            qc_vec_find_ ## TEST ## _prop,   \
            &qc_vec_info,                    \
            &qc_int_info)

QC_MKTEST_FIND(content);
QC_MKTEST_FIND(elem);
QC_MKTEST_FIND(iter);
QC_MKTEST_FIND(len);

QC_MKTEST_ALL(qc_vec_find_test_all,
        qc_vec_find_content_test,
        qc_vec_find_elem_test,
        qc_vec_find_iter_test,
        qc_vec_find_len_test,
        );
