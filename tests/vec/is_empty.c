#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(is_empty, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(is_empty, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(iter) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec cpy = *vec;

    vec_is_empty(vec);

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

    size_t cap = vec->capacity;

    bool res = vec_is_empty(vec);

    size_t nbytes = cap * sizeof(int);
    bool ret = res
        || memcmp(vec->ptr, dup.ptr, nbytes) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(len) (struct theft * t, void * arg1)
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

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(len);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(is_empty),
        QC_MKID_TEST(content),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(len),
        );
