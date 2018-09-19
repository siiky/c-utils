#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(qsort, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(qsort, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(is_sorted) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    vec_qsort(vec, qc_int_compar);

    return QC_BOOL2TRIAL(qc_vec_is_sorted(vec));
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec cpy = *vec;

    vec_qsort(vec, qc_int_compar);

    bool ret = memcmp(vec, &cpy, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_qsort(vec, qc_int_compar);

    bool ret = true;
    for (size_t i = 0; ret && i < dup.length; i++)
        ret = qc_vec_search(vec, dup.ptr[i], NULL);

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(is_sorted);
QC_MKTEST_FUNC(meta);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(qsort),
        QC_MKID_TEST(content),
        QC_MKID_TEST(is_sorted),
        QC_MKID_TEST(meta),
        );
