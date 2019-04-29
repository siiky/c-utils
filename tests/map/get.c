#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(get, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(get, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_map_info)

#define _QC_PRE() \
    if (qc_map_cardinal(map) == 0) return THEFT_TRIAL_SKIP; \
    int key = qc_map_random_in(t, map)

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    const struct map * map = arg1;
    _QC_PRE();
    struct map clone = {0};
    if (!qc_map_clone(map, &clone))
        return THEFT_TRIAL_SKIP;
    map_get(&clone, key);
    return QC_BOOL2TRIAL(qc_map_content_eq(map, &clone));
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    const struct map * map = arg1;
    struct map copy = *map;
    _QC_PRE();
    map_get(&copy, key);
    bool ret = qc_map_cardinal_eq(map, &copy)
        && qc_map_iter_eq(map, &copy);
    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1)
{
    const struct map * map = arg1;
    struct map copy = *map;
    _QC_PRE();
    int expected = qc_map_get(map, key);
    int got = map_get(&copy, key);
    return QC_BOOL2TRIAL(expected == got);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(get),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(res),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
