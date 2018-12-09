#include "map.h"

#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(get, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(get, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_map_info)

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1)
{
    const struct map * _map = arg1;

    if (qc_map_cardinal(_map) == 0)
        return THEFT_TRIAL_SKIP;

    int key = qc_map_random_in(t, _map);

    int expected = qc_map_get(_map, key);

    struct map map = *_map;
    int got = map_get(&map, key);

    bool res = expected == got;

    return QC_BOOL2TRIAL(res);
}

QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(get),
        QC_MKID_TEST(res),
        );
