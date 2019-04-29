#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(as_mut_slice, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(as_mut_slice, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop1,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info)

static enum theft_trial_res QC_MKID_PROP(ptr) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    int * pre_ptr = vec->ptr;
    int * res  = vec_as_mut_slice(vec);

    return QC_BOOL2TRIAL(pre_ptr == res);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec pre_dup = {0};
    if (!qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    vec_as_mut_slice(vec);

    size_t pos_len = vec->length;

    bool ret = pos_len == 0
        || memcmp(pre_dup.ptr, vec->ptr, pos_len * sizeof(int)) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1)
{
    UNUSED(t);

    struct vec * vec = arg1;

    struct vec cpy = *vec;

    vec_as_mut_slice(vec);

    bool res = memcmp(vec, &cpy, sizeof(struct vec)) == 0;

    return QC_BOOL2TRIAL(res);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(ptr);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(as_mut_slice),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(ptr),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
