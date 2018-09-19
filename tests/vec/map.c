#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(map, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(map, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static int _map_id (int elem)
{
    return elem;
}

static int _map_double (int elem)
{
    return elem * 2;
}

static enum theft_trial_res _qc_vec_map_meta_prop (struct theft * t, void * arg1, int (*f) (int))
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec cpy = *vec;

    vec_map(vec, f);

    bool ret = memcmp(vec, &cpy, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res _qc_vec_map_content_prop (struct theft * t, void * arg1, int (*f) (int))
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_map(vec, f);

    bool ret = true;
    size_t len = dup.length;
    for (size_t i = 0; i < len && ret; i++)
        ret = vec->ptr[i] == f(dup.ptr[i]);

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(id_meta) (struct theft * t, void * arg1)
{
    return _qc_vec_map_meta_prop(t, arg1, _map_id);
}

static enum theft_trial_res QC_MKID_PROP(double_meta) (struct theft * t, void * arg1)
{
    return _qc_vec_map_meta_prop(t, arg1, _map_double);
}

static enum theft_trial_res QC_MKID_PROP(id_content) (struct theft * t, void * arg1)
{
    return _qc_vec_map_content_prop(t, arg1, _map_id);
}

static enum theft_trial_res QC_MKID_PROP(double_content) (struct theft * t, void * arg1)
{
    return _qc_vec_map_content_prop(t, arg1, _map_double);
}

QC_MKTEST_FUNC(double_content);
QC_MKTEST_FUNC(double_meta);
QC_MKTEST_FUNC(id_content);
QC_MKTEST_FUNC(id_meta);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(map),
        QC_MKID_TEST(double_content),
        QC_MKID_TEST(double_meta),
        QC_MKID_TEST(id_content),
        QC_MKID_TEST(id_meta),
        );
