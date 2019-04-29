#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(iter_next, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(iter_next, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_iter_next(vec);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(idx_len) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    vec_iter_next(vec);

    return QC_BOOL2TRIAL((vec->length > 0) ?
            vec->idx < vec->length:
            vec->idx == 0);
}

static enum theft_trial_res QC_MKID_PROP(idx) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_idx = vec->idx;

    vec_iter_next(vec);

    size_t pos_idx = vec->idx;

    bool ret = (!vec->iterating) ?
        pos_idx == pre_idx:
        (vec->reverse) ?
        pos_idx <= pre_idx:
        pos_idx >= pre_idx;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(idx);
QC_MKTEST_FUNC(idx_len);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(iter_next),
        QC_MKID_TEST(content),
        QC_MKID_TEST(idx_len),
        QC_MKID_TEST(idx),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
