#include <stdbool.h>

#include "push.h"

#include <common.h>
#include <unused.h>

#include "vec.h"

static enum theft_trial_res qc_vec_push_len_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = (struct vec *) arg1;
    int elem = * (int *) arg2;

    size_t pre_len = vec_len(vec);

    bool res = vec_push(vec, elem);

    size_t pos_len = vec_len(vec);

    return ((res) ?
            (pos_len == (pre_len + 1)):
            (pos_len == pre_len)) ?
        THEFT_TRIAL_PASS:
        THEFT_TRIAL_FAIL;
}

static enum theft_trial_res qc_vec_push_elem_prop (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = (struct vec *) arg1;
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

    return ((res) ?
            (pos_last == elem):
            ((pre_empty && pos_empty) || (!pre_empty && !pos_empty && pre_last == pos_last))) ?
        THEFT_TRIAL_PASS:
        THEFT_TRIAL_FAIL;
}

qc_mktest(qc_vec_push_len_test,
        prop2,
        qc_vec_push_len_prop,
        &qc_vec_info,
        &qc_int_info
        );

qc_mktest(qc_vec_push_elem_test,
        prop2,
        qc_vec_push_elem_prop,
        &qc_vec_info,
        &qc_int_info
        );

qc_mktest_all(qc_vec_push_test_all,
        qc_vec_push_len_test,
        qc_vec_push_elem_test
        );
