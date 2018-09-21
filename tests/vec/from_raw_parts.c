#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(from_raw_parts, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(from_raw_parts, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop3,                \
            QC_MKID_PROP(TEST),   \
            &qc_void_ptr_info,    \
            &qc_size_t_info,      \
            &qc_size_t_info)

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    QC_ARG2VAR(1, int *, ptr);
    QC_ARG2VAR(2, size_t, length);
    QC_ARG2VAR(3, size_t, capacity);

    struct vec vec = {0};

    vec_from_raw_parts(&vec, ptr, length, capacity);

    bool ret = true
        && !vec.iterating
        && !vec.reverse
        && capacity == vec.capacity
        && length == vec.length
        && ptr == vec.ptr
        && vec.idx == 0;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    QC_ARG2VAR(1, int *, ptr);
    QC_ARG2VAR(2, size_t, length);
    QC_ARG2VAR(3, size_t, capacity);

    struct vec vec = {0};

    bool ret = vec_from_raw_parts(&vec, ptr, length, capacity);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(from_raw_parts),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(res),
        );
