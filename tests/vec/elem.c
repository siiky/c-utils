#include "vec.h"

static enum theft_trial_res qc_vec_elem_len_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    size_t pre_len = vec->length;

    vec_elem(vec, elem);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res qc_vec_elem_elem_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    size_t i = 0;
    bool res1 = vec_elem(vec, elem);
    bool res2 = qc_vec_search(vec, elem, &i);

    return QC_BOOL2TRIAL(res1 == res2);
}

static enum theft_trial_res qc_vec_elem_content_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    struct vec * pre_dup = qc_vec_dup_contents(vec);
    if (pre_dup == NULL)
        return THEFT_TRIAL_SKIP;

    size_t pre_len = vec->length;

    vec_elem(vec, elem);

    size_t nbytes = pre_len * sizeof(int);
    bool ret = memcmp(pre_dup->ptr, vec->ptr, nbytes) == 0;

    qc_vec_dup_free(pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_elem_iter_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_elem(vec, elem);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_ELEM(TEST)                 \
    QC_MKTEST(qc_vec_elem_ ## TEST ## _test, \
            prop2,                           \
            qc_vec_elem_ ## TEST ## _prop,   \
            &qc_vec_info,                    \
            &qc_int_info)

QC_MKTEST_ELEM(content);
QC_MKTEST_ELEM(elem);
QC_MKTEST_ELEM(iter);
QC_MKTEST_ELEM(len);

QC_MKTEST_ALL(qc_vec_elem_test_all,
        qc_vec_elem_content_test,
        qc_vec_elem_elem_test,
        qc_vec_elem_iter_test,
        qc_vec_elem_len_test,
        );
