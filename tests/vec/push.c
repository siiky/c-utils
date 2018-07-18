#include <stdbool.h>
#include <string.h>

#include <common.h>
#include <unused.h>

#include "vec.h"

static enum theft_trial_res qc_vec_push_len_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    size_t pre_len = vec->length;

    bool res = vec_push(vec, elem);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL((res) ?
            (pos_len == (pre_len + 1)):
            (pos_len == pre_len));
}

static enum theft_trial_res qc_vec_push_last_elem_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    bool pre_empty = vec->length == 0;
    int pre_last = (!pre_empty) ?
        vec->ptr[vec->length - 1]:
        0;

    bool res = vec_push(vec, elem);

    bool pos_empty = vec->length == 0;
    int pos_last = (!pos_empty) ?
        vec->ptr[vec->length - 1]:
        0;

    return QC_BOOL2TRIAL((res) ?
            (pos_last == elem):
            ((pre_empty && pos_empty) || (!pre_empty && !pos_empty && pre_last == pos_last)));
}

static enum theft_trial_res qc_vec_push_content_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    struct vec * pre_dup = qc_vec_dup_contents(vec);
    if (pre_dup == NULL)
        return THEFT_TRIAL_SKIP;

    size_t pre_len = vec->length;
    size_t pre_cap = vec->capacity;

    bool succ = vec_push(vec, elem);

    size_t nbytes = sizeof(int) * ((succ) ?
            pre_len:
            pre_cap);

    bool ret = memcmp(pre_dup->ptr, vec->ptr, nbytes) == 0;

    qc_vec_dup_free(pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_push_iter_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    int elem = * (int *) arg2;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_push(vec, elem);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_PUSH(TEST)                 \
    QC_MKTEST(qc_vec_push_ ## TEST ## _test, \
            prop2,                           \
            qc_vec_push_ ## TEST ## _prop,   \
            &qc_vec_info,                    \
            &qc_int_info)

QC_MKTEST_PUSH(content);
QC_MKTEST_PUSH(iter);
QC_MKTEST_PUSH(last_elem);
QC_MKTEST_PUSH(len);

QC_MKTEST_ALL(qc_vec_push_test_all,
        qc_vec_push_content_test,
        qc_vec_push_iter_test,
        qc_vec_push_last_elem_test,
        qc_vec_push_len_test,
        );
