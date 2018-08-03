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

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    size_t cap = vec->capacity;

    bool res = vec_is_empty(vec);

    size_t nbytes = cap * sizeof(int);
    bool ret = res
        || memcmp(vec->ptr, dup.ptr, nbytes) == 0;

    qc_vec_dup_free(&dup);

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

#define QC_MKID_FUNC(TEST, TYPE) \
    QC_MKID_MOD(is_empty, TEST, TYPE)

#define QC_MKID_PROP(TEST) \
    QC_MKID_FUNC(TEST, prop)

#define QC_MKID_TEST(TEST) \
    QC_MKID_FUNC(TEST, test)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(len);

QC_MKTEST_ALL(qc_vec_is_empty_test_all,
        QC_MKID_TEST(content),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(len),
        );
