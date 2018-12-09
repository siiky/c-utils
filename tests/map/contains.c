#include "map.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(contains, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(contains, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_map_info,         \
            &qc_int_info)

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    const struct map * _map = arg1;
    QC_ARG2VAR(2, int, key);

    bool res1 = qc_map_contains(_map, key);

    struct map map = *_map;
    bool res2 = map_contains(&map, key);

    bool ret = (res1 && res2) || (!res1 && !res2);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res_not_in) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    const struct map * _map = arg1;
    QC_ARG2VAR(2, int, key);

    key = qc_map_random_not_in(_map, key);

    QC_ARG2VAL(2, int) = key;

    struct map map = *_map;
    bool ret = !map_contains(&map, key);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res_in) (struct theft * t, void * arg1, void * arg2)
{
    const struct map * _map = arg1;
    QC_ARG2VAR(2, int, key);

    if (qc_map_cardinal(_map) == 0)
        return THEFT_TRIAL_SKIP;

    key = qc_map_random_in(t, _map);

    QC_ARG2VAL(2, int) = key;

    struct map map = *_map;
    bool ret = map_contains(&map, key);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(res);
QC_MKTEST_FUNC(res_in);
QC_MKTEST_FUNC(res_not_in);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(contains),
        QC_MKID_TEST(res),
        QC_MKID_TEST(res_in),
        QC_MKID_TEST(res_not_in),
        );
