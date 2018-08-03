#include "vec.h"

static enum theft_trial_res qc_vec_itering_content_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_itering(vec);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_itering_res_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    bool itering = vec->iterating;

    bool res = vec_itering(vec);

    bool ret = itering == res;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_itering_iter_prop (struct theft * t, void * arg1)
{
    UNUSED(t);
    struct vec * vec = arg1;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_itering(vec);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKID_FUNC(TEST, TYPE) \
    QC_MKID_MOD(itering, TEST, TYPE)

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
QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(qc_vec_itering_test_all,
        QC_MKID_TEST(content),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(res),
        );
