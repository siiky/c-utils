#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(push, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(push, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_int_info)

static enum theft_trial_res QC_MKID_PROP(len) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    size_t pre_len = vec->length;

    bool res = vec_push(vec, elem);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL((res) ?
            (pos_len == (pre_len + 1)):
            (pos_len == pre_len));
}

static enum theft_trial_res QC_MKID_PROP(last_elem) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    bool pre_empty = vec->length == 0;
    int pre_last = (!pre_empty) ?
        vec->ptr[vec->length - 1]:
        0;

    bool res = vec_push(vec, elem);

    bool pos_empty = vec->length == 0;
    int pos_last = (!pos_empty) ?
        vec->ptr[vec->length - 1]:
        0;

    return QC_BOOL2TRIAL((res) ?
            (pos_last == elem):
            ((pre_empty && pos_empty) || (!pre_empty && !pos_empty && pre_last == pos_last)));
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    struct vec pre_dup = {0};
    if (!qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    size_t pre_len = vec->length;
    size_t pre_cap = vec->capacity;

    bool succ = vec_push(vec, elem);

    size_t nbytes = sizeof(int) * ((succ) ?
            pre_len:
            pre_cap);

    bool ret = memcmp(pre_dup.ptr, vec->ptr, nbytes) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(iter) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, int, elem);

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_push(vec, elem);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool ret = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(last_elem);
QC_MKTEST_FUNC(len);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(push),
        QC_MKID_TEST(content),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(last_elem),
        QC_MKID_TEST(len),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
