#include "vec.h"

static size_t what = 0;

static void qc_vec_foreach_range_do_nothing_func (const int x)
{
    UNUSED(x);
    what++;
}

static enum theft_trial_res qc_vec_foreach_range_len_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);
    struct vec * vec = arg1;
    size_t from = * (size_t *) arg2;
    size_t to = * (size_t *) arg3;

    size_t pre_len = vec->length;
    vec_foreach_range(vec, qc_vec_foreach_range_do_nothing_func, from, to);
    what = 0;
    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res qc_vec_foreach_range_iter_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t from = * (size_t *) arg2;
    size_t to = * (size_t *) arg3;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_foreach_range(vec, qc_vec_foreach_range_do_nothing_func, from, to);
    what = 0;

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res qc_vec_foreach_range_content_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    size_t from = * (size_t *) arg2;
    size_t to = * (size_t *) arg3;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    size_t pre_len = vec->length;
    vec_foreach_range(vec, qc_vec_foreach_range_do_nothing_func, from, to);
    what = 0;

    bool res = pre_len == 0
        || memcmp(dup.ptr, vec->ptr, pre_len * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res qc_vec_foreach_range_side_effects_prop (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);
    struct vec * vec = arg1;
    size_t from = * (size_t *) arg2;
    size_t to = * (size_t *) arg3;

    size_t pre_len = vec->length;
    bool res = vec_foreach_range(vec, qc_vec_foreach_range_do_nothing_func, from, to);
    bool ret = !res || what == pre_len;
    what = 0;

    return QC_BOOL2TRIAL(ret);
}

#define QC_MKID_FUNC(TEST, TYPE) \
    QC_MKID_MOD(foreach_range, TEST, TYPE)

#define QC_MKID_PROP(TEST) \
    QC_MKID_FUNC(TEST, prop)

#define QC_MKID_TEST(TEST) \
    QC_MKID_FUNC(TEST, test)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop3,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info,      \
            &qc_size_t_info)

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(len);
QC_MKTEST_FUNC(side_effects);

QC_MKTEST_ALL(qc_vec_foreach_range_test_all,
        QC_MKID_TEST(content),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(len),
        QC_MKID_TEST(side_effects),
        );
