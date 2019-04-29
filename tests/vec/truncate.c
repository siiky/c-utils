#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(truncate, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(truncate, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info)

/* IB tests */
static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, len);

    {
        size_t _len = vec->length;
        if (len >= _len) {
            len = len % (_len + 1);
            QC_ARG2VAL(2, size_t) = len;
        }
    }

    bool pre_itering = vec->iterating;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;
    size_t pre_rev = vec->reverse;

    vec_truncate(vec, len);

    bool pos_itering = vec->iterating;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;
    size_t pos_rev = vec->reverse;

    bool ret = true
        && pre_cap >= pos_cap
        && pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_len >= pos_len
        && pre_rev == pos_rev;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, len);

    {
        size_t _len = vec->length;
        if (len >= _len) {
            len = len % (_len + 1);
            QC_ARG2VAL(2, size_t) = len;
        }
    }

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_truncate(vec, len);

    bool ret = memcmp(vec->ptr, dup.ptr, vec->length * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

/* OOB tests */
static enum theft_trial_res QC_MKID_PROP(oob_meta) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, len);

    bool pre_itering = vec->iterating;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;
    size_t pre_rev = vec->reverse;

    vec_truncate(vec, len);

    bool pos_itering = vec->iterating;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;
    size_t pos_rev = vec->reverse;

    bool ret = true
        && pre_cap >= pos_cap
        && pre_idx == pos_idx
        && pre_itering == pos_itering
        && pre_len >= pos_len
        && pre_rev == pos_rev;

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(oob_content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, len);

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_truncate(vec, len);

    bool ret = memcmp(vec->ptr, dup.ptr, vec->length * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(oob_content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(oob_meta);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(truncate),
        QC_MKID_TEST(content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(oob_content),
        QC_MKID_TEST(oob_meta),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
