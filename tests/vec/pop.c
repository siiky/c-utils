#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(pop, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(pop, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(len) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    vec_pop(vec);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == (pos_len + 1));
}

static enum theft_trial_res QC_MKID_PROP(elem) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    int pre_elem = vec->ptr[pre_len - 1];

    int elem = vec_pop(vec);

    return QC_BOOL2TRIAL(elem == pre_elem);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    struct vec pre_dup = {0};
    if (pre_len == 0 || !qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;


    vec_pop(vec);

    size_t pos_len = vec->length;

    bool ret = pos_len == 0
        || memcmp(pre_dup.ptr, vec->ptr, pos_len * sizeof(int)) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(iter) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_pop(vec);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool res = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(res);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(elem);
QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(len);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(pop),
        QC_MKID_TEST(content),
        QC_MKID_TEST(elem),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(len),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
