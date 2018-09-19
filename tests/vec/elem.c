#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(elem, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(elem, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_int_info)

static enum theft_trial_res QC_MKID_PROP(len) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    size_t pre_len = vec->length;

    vec_elem(vec, elem);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res QC_MKID_PROP(elem) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    size_t i = 0;
    bool res1 = vec_elem(vec, elem);
    bool res2 = qc_vec_search(vec, elem, &i);

    return QC_BOOL2TRIAL(res1 == res2);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    struct vec pre_dup = {0};
    if (!qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    size_t pre_len = vec->length;

    vec_elem(vec, elem);

    size_t nbytes = pre_len * sizeof(int);
    bool ret = memcmp(pre_dup.ptr, vec->ptr, nbytes) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    struct vec cpy = *vec;

    vec_elem(vec, elem);

    bool ret = memcmp(vec, &cpy, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(elem);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(len);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(elem),
        QC_MKID_TEST(content),
        QC_MKID_TEST(elem),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(len),
        );
