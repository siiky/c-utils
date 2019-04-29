#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(iter_rev, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(iter_rev, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_bool_info)

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, bool, rev);

    bool itering = vec->iterating;

    bool res = vec_iter_rev(vec, rev);

    bool ret = (itering) ?
        !res:
        res;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, bool, rev);

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_iter_rev(vec, rev);

    bool ret = memcmp(vec->ptr, dup.ptr, dup.capacity) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, bool, rev);

    bool pre_itering = vec->iterating;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    vec_iter_rev(vec, rev);

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

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(iter_rev),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(res),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
