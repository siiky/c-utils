#include <stdbool.h>
#include <string.h>

#include "push.h"

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

    bool ret = true
        && pre_dup->iterating == vec->iterating
        && pre_dup->reverse   == vec->reverse
        && pre_dup->idx       == vec->idx
        && ((succ) ?
                memcmp(pre_dup->ptr, vec->ptr, pre_len * sizeof(int)) == 0:
                memcmp(pre_dup->ptr, vec->ptr, pre_cap * sizeof(int)) == 0);

    *pre_dup = vec_free(*pre_dup);
    free(pre_dup);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST(qc_vec_push_len_test,
        prop2,
        qc_vec_push_len_prop,
        &qc_vec_info,
        &qc_int_info
        );

QC_MKTEST(qc_vec_push_last_elem_test,
        prop2,
        qc_vec_push_last_elem_prop,
        &qc_vec_info,
        &qc_int_info
        );

QC_MKTEST(qc_vec_push_content_test,
        prop2,
        qc_vec_push_content_prop,
        &qc_vec_info,
        &qc_int_info
        );

QC_MKTEST_ALL(qc_vec_push_test_all,
        qc_vec_push_len_test,
        qc_vec_push_last_elem_test,
        qc_vec_push_content_test
        );
