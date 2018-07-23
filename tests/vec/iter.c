#include "vec.h"

static enum theft_trial_res qc_vec_iter_content_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;
    struct vec * dup = qc_vec_dup_contents(vec);
    if (dup == NULL)
        return THEFT_TRIAL_SKIP;
    size_t len = vec->length;

    vec_iter(vec);

    bool ret = len == 0
        || memcmp(vec->ptr, dup->ptr, dup->capacity * sizeof(int)) == 0;

    qc_vec_dup_free(dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_iter_iter_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t len = vec->length;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    bool res = vec_iter(vec);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = pre_reverse == pos_reverse
        && ((res) ?
                (pos_iterating
                 && pos_idx == ((pre_reverse) ? len - 1 : 0)):
                (pre_iterating == pos_iterating
                 && pre_idx    == pos_idx));

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_iter_meta_prop (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t pre_cap = vec->capacity;
    size_t pre_len = vec->length;
    void * pre_ptr = vec->ptr;

    vec_iter(vec);

    size_t pos_cap = vec->capacity;
    size_t pos_len = vec->length;
    void * pos_ptr = vec->ptr;

    bool ret = true
        && pre_cap == pos_cap
        && pre_len == pos_len
        && pre_ptr == pos_ptr;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKTEST_ITER(TEST)                 \
    QC_MKTEST(qc_vec_iter_ ## TEST ## _test, \
            prop1,                           \
            qc_vec_iter_ ## TEST ## _prop,   \
            &qc_vec_info)

QC_MKTEST_ITER(content);
QC_MKTEST_ITER(iter);
QC_MKTEST_ITER(meta);

QC_MKTEST_ALL(qc_vec_iter_test_all,
        qc_vec_iter_content_test,
        qc_vec_iter_iter_test,
        qc_vec_iter_meta_test,
        );
