#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(bsearch, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(bsearch, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_int_info)

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;
    void * pre_ptr = vec->ptr;

    vec_bsearch(vec, elem, qc_int_compar);

    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;
    void * pos_ptr = vec->ptr;

    bool ret = true
        && pre_cap       == pos_cap
        && pre_idx       == pos_idx
        && pre_iterating == pos_iterating
        && pre_len       == pos_len
        && pre_ptr       == pos_ptr
        && pre_reverse   == pos_reverse;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_bsearch(vec, elem, qc_int_compar);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    size_t len = vec->length;
    size_t idx = 0;
    bool exists = qc_vec_search(vec, elem, &idx);

    size_t res = vec_bsearch(vec, elem, qc_int_compar);

    bool ret = (exists) ?
        res == idx:
        res == len;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(bsearch),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(res),
        );
