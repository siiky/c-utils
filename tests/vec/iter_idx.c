#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(iter_idx, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(iter_idx, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

#define _QC_PRE()                                      \
    do {                                               \
        size_t len = vec->length;                      \
        if (len == 0) return THEFT_TRIAL_SKIP;         \
        if (vec->idx > len) vec->idx = vec->idx % len; \
    } while (0)

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    _QC_PRE();

    struct vec cpy = *vec;

    vec_iter_idx(vec);

    bool ret = memcmp(vec, &cpy, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    _QC_PRE();

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_iter_idx(vec);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.length * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    _QC_PRE();

    size_t idx = vec->idx;
    size_t res = vec_iter_idx(vec);

    bool ret = res == idx;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(iter_idx),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(res),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
#undef _QC_PRE
