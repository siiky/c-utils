#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(map_range, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(map_range, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop3,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info,      \
            &qc_size_t_info)

static int _map_id (int elem)
{
    return elem;
}

static int _map_double (int elem)
{
    return elem * 2;
}

static enum theft_trial_res _qc_vec_map_range_meta_prop (struct theft * t, void * arg1, int (*f) (int), void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    if (to > vec->length) {
        to = to % (vec->length + 1);

        QC_ARG2VAL(3, size_t) = to;
    }

    if (from >= to) {
        from = (to > 0) ?
            from % to:
            0;

        QC_ARG2VAL(2, size_t) = from;
    }

    struct vec cpy = *vec;

    vec_map_range(vec, f, from, to);

    bool ret = memcmp(vec, &cpy, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res _qc_vec_map_range_mapped_content_prop (struct theft * t, void * arg1, int (*f) (int), void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    if (to > vec->length) {
        to = to % (vec->length + 1);

        QC_ARG2VAL(3, size_t) = to;
    }

    if (from >= to) {
        from = (to > 0) ?
            from % to:
            0;

        QC_ARG2VAL(2, size_t) = from;
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_map_range(vec, f, from, to);

    bool ret = true;
    for (size_t i = from; i < to && ret; i++)
        ret = vec->ptr[i] == f(dup.ptr[i]);

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res _qc_vec_map_range_left_content_prop (struct theft * t, void * arg1, int (*f) (int), void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    if (to > vec->length) {
        to = to % (vec->length + 1);

        QC_ARG2VAL(3, size_t) = to;
    }

    if (from >= to) {
        from = (to > 0) ?
            from % to:
            0;

        QC_ARG2VAL(2, size_t) = from;
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_map_range(vec, f, from, to);

    bool ret = false
        || memcmp(vec->ptr, dup.ptr, from * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res _qc_vec_map_range_right_content_prop (struct theft * t, void * arg1, int (*f) (int), void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    if (to > vec->length) {
        to = to % (vec->length + 1);

        QC_ARG2VAL(3, size_t) = to;
    }

    if (from >= to) {
        from = (to > 0) ?
            from % to:
            0;

        QC_ARG2VAL(2, size_t) = from;
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_map_range(vec, f, from, to);

    bool ret = false
        || memcmp(vec->ptr + to, dup.ptr + to, (dup.length - to) * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(id_meta) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    return _qc_vec_map_range_meta_prop(t, arg1, _map_id, arg2, arg3);
}

static enum theft_trial_res QC_MKID_PROP(double_meta) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    return _qc_vec_map_range_meta_prop(t, arg1, _map_double, arg2, arg3);
}

static enum theft_trial_res QC_MKID_PROP(id_mapped_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    return _qc_vec_map_range_mapped_content_prop(t, arg1, _map_id, arg2, arg3);
}

static enum theft_trial_res QC_MKID_PROP(double_mapped_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    return _qc_vec_map_range_mapped_content_prop(t, arg1, _map_double, arg2, arg3);
}

static enum theft_trial_res QC_MKID_PROP(id_left_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    return _qc_vec_map_range_left_content_prop(t, arg1, _map_id, arg2, arg3);
}

static enum theft_trial_res QC_MKID_PROP(double_left_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    return _qc_vec_map_range_left_content_prop(t, arg1, _map_double, arg2, arg3);
}

static enum theft_trial_res QC_MKID_PROP(id_right_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    return _qc_vec_map_range_right_content_prop(t, arg1, _map_id, arg2, arg3);
}

static enum theft_trial_res QC_MKID_PROP(double_right_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    return _qc_vec_map_range_right_content_prop(t, arg1, _map_double, arg2, arg3);
}

QC_MKTEST_FUNC(double_left_content);
QC_MKTEST_FUNC(double_mapped_content);
QC_MKTEST_FUNC(double_meta);
QC_MKTEST_FUNC(double_right_content);
QC_MKTEST_FUNC(id_left_content);
QC_MKTEST_FUNC(id_mapped_content);
QC_MKTEST_FUNC(id_meta);
QC_MKTEST_FUNC(id_right_content);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(map_range),
        QC_MKID_TEST(double_left_content),
        QC_MKID_TEST(double_mapped_content),
        QC_MKID_TEST(double_meta),
        QC_MKID_TEST(double_right_content),
        QC_MKID_TEST(id_left_content),
        QC_MKID_TEST(id_mapped_content),
        QC_MKID_TEST(id_meta),
        QC_MKID_TEST(id_right_content),
        );
