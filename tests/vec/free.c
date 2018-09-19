#include "vec.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(free, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(free, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(all) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    struct vec empty = {0};

    dup = vec_free(dup);

    bool ret = memcmp(&dup, &empty, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(all);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(free),
        QC_MKID_TEST(all),
        );
