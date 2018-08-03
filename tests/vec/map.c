#include "vec.h"

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

    bool pre_itering = vec->iterating;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    vec_map(vec, f);

    bool pos_itering = vec->iterating;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;

    bool ret = true
        && pre_cap == pos_cap
        && pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_len == pos_len;

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

static enum theft_trial_res qc_vec_map_id_meta_prop (struct theft * t, void * arg1)
{
    return _qc_vec_map_meta_prop(t, arg1, _map_id);
}

static enum theft_trial_res qc_vec_map_double_meta_prop (struct theft * t, void * arg1)
{
    return _qc_vec_map_meta_prop(t, arg1, _map_double);
}

static enum theft_trial_res qc_vec_map_id_content_prop (struct theft * t, void * arg1)
{
    return _qc_vec_map_content_prop(t, arg1, _map_id);
}

static enum theft_trial_res qc_vec_map_double_content_prop (struct theft * t, void * arg1)
{
    return _qc_vec_map_content_prop(t, arg1, _map_double);
}

#define QC_MKID_VEC_MAP(TEST, TYPE) \
    QC_MKID_VEC(map, TEST, TYPE)

#define QC_MKID_VEC_MAP_PROP(TEST) \
    QC_MKID_VEC_MAP(TEST, prop)

#define QC_MKID_VEC_MAP_TEST(TEST) \
    QC_MKID_VEC_MAP(TEST, test)

#define QC_MKTEST_MAP(TEST)               \
    QC_MKTEST(QC_MKID_VEC_MAP_TEST(TEST), \
            prop1,                        \
            QC_MKID_VEC_MAP_PROP(TEST),   \
            &qc_vec_info)

QC_MKTEST_MAP(double_content);
QC_MKTEST_MAP(double_meta);
QC_MKTEST_MAP(id_content);
QC_MKTEST_MAP(id_meta);

QC_MKTEST_ALL(qc_vec_map_test_all,
        QC_MKID_VEC_MAP_TEST(double_content),
        QC_MKID_VEC_MAP_TEST(double_meta),
        QC_MKID_VEC_MAP_TEST(id_content),
        QC_MKID_VEC_MAP_TEST(id_meta),
        );
