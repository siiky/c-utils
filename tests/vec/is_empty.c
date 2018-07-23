#include "vec.h"

static enum theft_trial_res qc_vec_is_empty_iter_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_is_empty(vec);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_is_empty_content_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec * dup = qc_vec_dup_contents(vec);
    if (dup == NULL)
        return THEFT_TRIAL_SKIP;

    size_t cap = vec->capacity;

    bool res = vec_is_empty(vec);

    size_t nbytes = cap * sizeof(int);
    bool ret = res
        || memcmp(vec->ptr, dup->ptr, nbytes) == 0;

    qc_vec_dup_free(dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_is_empty_len_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_len = vec->length;

    bool res = vec_is_empty(vec);

    size_t pos_len = vec->length;

    bool ret = pre_len == pos_len
        && ((res) ?
                pre_len == 0:
                pre_len >  0);

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_IS_EMPTY(TEST)                 \
    QC_MKTEST(qc_vec_is_empty_ ## TEST ## _test, \
            prop1,                               \
            qc_vec_is_empty_ ## TEST ## _prop,   \
            &qc_vec_info)

QC_MKTEST_IS_EMPTY(content);
QC_MKTEST_IS_EMPTY(iter);
QC_MKTEST_IS_EMPTY(len);

QC_MKTEST_ALL(qc_vec_is_empty_test_all,
        qc_vec_is_empty_content_test,
        qc_vec_is_empty_iter_test,
        qc_vec_is_empty_len_test,
        );
