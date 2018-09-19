#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(foreach_range, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(foreach_range, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop3,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info,      \
            &qc_size_t_info)

static size_t what = 0;

static void _do_nothing (const int x)
{
    UNUSED(x);
    what++;
}

static enum theft_trial_res QC_MKID_PROP(len) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    size_t pre_len = vec->length;
    vec_foreach_range(vec, _do_nothing, from, to);
    what = 0;
    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res QC_MKID_PROP(iter) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    struct vec cpy = *vec;

    vec_foreach_range(vec, _do_nothing, from, to);
    what = 0;

    bool ret = memcmp(vec, &cpy, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    size_t pre_len = vec->length;
    vec_foreach_range(vec, _do_nothing, from, to);
    what = 0;

    bool res = pre_len == 0
        || memcmp(dup.ptr, vec->ptr, pre_len * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res QC_MKID_PROP(side_effects) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    size_t pre_len = vec->length;
    bool res = vec_foreach_range(vec, _do_nothing, from, to);
    bool ret = !res || what == pre_len;
    what = 0;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(len);
QC_MKTEST_FUNC(side_effects);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(foreach_range),
        QC_MKID_TEST(content),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(len),
        QC_MKID_TEST(side_effects),
        );
