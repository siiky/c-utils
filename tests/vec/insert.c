#include <stdbool.h>
#include <string.h>

#include "vec.h"

#include <common.h>
#include <unused.h>

static enum theft_trial_res qc_vec_insert_len_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;
    int elem = * (int *) arg3;

    size_t pre_len = vec->length;
    if (idx > pre_len) {
        idx = (pre_len > 0) ?
            idx % pre_len:
            0;
        * (size_t *) arg2 = idx;
    }

    bool succ = vec_insert(vec, idx, elem);
    size_t pos_len = vec->length;

    bool res = (succ) ?
        pos_len == (pre_len + 1):
        pos_len == pre_len;

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res qc_vec_insert_left_content_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;
    int elem = * (int *) arg3;

    size_t pre_len = vec->length;
    if (idx > pre_len) {
        idx = (pre_len > 0) ?
            idx % pre_len:
            0;
        * (size_t *) arg2 = idx;
    }

    struct vec * dup = qc_vec_dup_contents(vec);
    if (dup == NULL)
        return THEFT_TRIAL_SKIP;

    vec_insert(vec, idx, elem);

    size_t nbytes = idx * sizeof(int);
    bool res = idx == 0
        || memcmp(vec->ptr, dup->ptr, nbytes) == 0;

    qc_vec_dup_free(dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res qc_vec_insert_elem_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;
    int elem = * (int *) arg3;

    size_t pre_len = vec->length;
    if (idx > pre_len) {
        idx = (pre_len > 0) ?
            idx % pre_len:
            0;
        * (size_t *) arg2 = idx;
    }

    int pre_elem = (idx < pre_len) ?
        vec->ptr[idx]:
        0;
    bool succ = vec_insert(vec, idx, elem);
    size_t pos_len = vec->length;
    int pos_elem = (idx < pos_len) ?
        vec->ptr[idx]:
        0;

    bool res = (succ) ?
        pos_elem == elem:
        pos_elem == pre_elem;

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res qc_vec_insert_right_content_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;
    int elem = * (int *) arg3;

    struct vec * dup = qc_vec_dup_contents(vec);
    if (dup == NULL)
        return THEFT_TRIAL_SKIP;

    size_t pre_len = vec->length;
    if (idx > pre_len) {
        idx = (pre_len > 0) ?
            idx % pre_len:
            0;
        * (size_t *) arg2 = idx;
    }

    bool succ = vec_insert(vec, idx, elem);
    size_t pos_len = vec->length;

    bool res = idx >= pos_len
        || memcmp(dup->ptr + idx,
                vec->ptr + idx + ((succ) ? 1 : 0),
                (dup->length - idx) * sizeof(int)) == 0;

    qc_vec_dup_free(dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res qc_vec_insert_iter_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t idx = * (size_t *) arg2;
    int elem = * (int *) arg3;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_insert(vec, idx, elem);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_INSERT(TEST)                 \
    QC_MKTEST(qc_vec_insert_ ## TEST ## _test, \
            prop3,                             \
            qc_vec_insert_ ## TEST ## _prop,   \
            &qc_vec_info,                      \
            &qc_size_t_info,                   \
            &qc_int_info)

QC_MKTEST_INSERT(elem);
QC_MKTEST_INSERT(iter);
QC_MKTEST_INSERT(left_content);
QC_MKTEST_INSERT(len);
QC_MKTEST_INSERT(right_content);

QC_MKTEST_ALL(qc_vec_insert_test_all,
        qc_vec_insert_elem_test,
        qc_vec_insert_iter_test,
        qc_vec_insert_left_content_test,
        qc_vec_insert_len_test,
        qc_vec_insert_right_content_test,
        );
