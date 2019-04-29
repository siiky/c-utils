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

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    struct map * map = arg1;
    QC_ARG2VAR(2, int, key);
    struct map clone = {0};
    if (!qc_map_clone(map, &clone))
        return THEFT_TRIAL_SKIP;
    map_contains(&clone, key);
    return QC_BOOL2TRIAL(qc_map_content_eq(map, &clone));
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    struct map * map = arg1;
    struct map copy = *map;
    QC_ARG2VAR(2, int, key);
    map_contains(&copy, key);
    bool ret = qc_map_cardinal_eq(map, &copy)
        && qc_map_iter_eq(map, &copy);
    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    const struct map * map = arg1;
    QC_ARG2VAR(2, int, key);
    bool res1 = qc_map_contains(map, key);
    struct map copy = *map;
    bool res2 = map_contains(&copy, key);
    bool ret = (!!res1) == (!!res2);
    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res_not_in) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    const struct map * map = arg1;
    QC_ARG2VAR(2, int, key);
    key = qc_map_random_not_in(map, key);
    QC_ARG2VAL(2, int) = key;
    struct map copy = *map;
    bool ret = !map_contains(&copy, key);
    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res_in) (struct theft * t, void * arg1, void * arg2)
{
    const struct map * map = arg1;
    QC_ARG2VAR(2, int, key);
    if (qc_map_cardinal(map) == 0)
        return THEFT_TRIAL_SKIP;
    key = qc_map_random_in(t, map);
    QC_ARG2VAL(2, int) = key;
    struct map copy = *map;
    bool ret = map_contains(&copy, key);
    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(res);
QC_MKTEST_FUNC(res_in);
QC_MKTEST_FUNC(res_not_in);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(contains),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(res),
        QC_MKID_TEST(res_in),
        QC_MKID_TEST(res_not_in),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
