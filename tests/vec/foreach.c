#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(foreach, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(foreach, TEST)

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
    vec_foreach(vec, _do_nothing);
    what = 0;
    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == pos_len);
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec cpy = *vec;

    vec_foreach(vec, _do_nothing);
    what = 0;

    bool ret = memcmp(vec, &cpy, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    size_t pre_cap = vec->capacity;
    vec_foreach(vec, _do_nothing);
    what = 0;

    bool res = false
        || memcmp(dup.ptr, vec->ptr, pre_cap * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res QC_MKID_PROP(side_effects) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    size_t pre_len = vec->length;
    vec_foreach(vec, _do_nothing);
    bool res = what == pre_len;
    what = 0;

    return QC_BOOL2TRIAL(res);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(len);
QC_MKTEST_FUNC(side_effects);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(foreach),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(len),
        QC_MKID_TEST(side_effects),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
