#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(remove, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(remove, TEST)

#define QC_MKTEST_FUNC(TEST)       \
    QC_MKTEST(QC_MKID_TEST(TEST),  \
            prop2,                 \
            QC_MKID_PROP(TEST),    \
            &qc_vec_info,          \
            &qc_size_t_info)

static enum theft_trial_res QC_MKID_PROP(len) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, idx);

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        QC_ARG2VAL(2, size_t) = idx;
    }

    vec_remove(vec, idx);

    size_t pos_len = vec->length;

    return QC_BOOL2TRIAL(pre_len == (pos_len + 1));
}

static enum theft_trial_res QC_MKID_PROP(elem) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, idx);

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        QC_ARG2VAL(2, size_t) = idx;
    }

    int pre_elem = vec->ptr[idx];

    int elem = vec_remove(vec, idx);

    return QC_BOOL2TRIAL(pre_elem == elem);
}

static enum theft_trial_res QC_MKID_PROP(left_content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, idx);

    struct vec pre_dup = {0};
    size_t pre_len = vec->length;
    if (pre_len == 0 || !qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        QC_ARG2VAL(2, size_t) = idx;
    }

    vec_remove(vec, idx);

    void * pre = pre_dup.ptr;
    void * pos = vec->ptr;
    size_t nbytes = idx * sizeof(int);

    bool res = idx == 0 /* no content on the left */
        || memcmp(pos, pre, nbytes) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res QC_MKID_PROP(right_content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, idx);

    struct vec pre_dup = {0};
    size_t pre_len = vec->length;
    if (pre_len == 0 || !qc_vec_dup_contents(vec, &pre_dup))
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        QC_ARG2VAL(2, size_t) = idx;
    }

    vec_remove(vec, idx);
    size_t pos_len = vec->length;

    void * pre = pre_dup.ptr + idx + 1;
    void * pos = vec->ptr + idx;
    size_t nbytes = (pos_len - idx) * sizeof(int);

    bool res = idx >= pos_len /* no content on the right */
        || memcmp(pre, pos, nbytes) == 0;

    qc_vec_dup_free(&pre_dup);

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res QC_MKID_PROP(iter) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, idx);

    size_t pre_len = vec->length;
    if (pre_len == 0)
        return THEFT_TRIAL_SKIP;

    if (idx >= pre_len) {
        idx = idx % pre_len;
        QC_ARG2VAL(2, size_t) = idx;
    }

    size_t pre_idx = vec->idx;
    unsigned char pre_iterating = vec->iterating;
    unsigned char pre_reverse = vec->reverse;

    vec_remove(vec, idx);

    size_t pos_idx = vec->idx;
    unsigned char pos_iterating = vec->iterating;
    unsigned char pos_reverse = vec->reverse;

    bool res = true
        && pre_iterating == pos_iterating
        && pre_reverse   == pos_reverse
        && pre_idx       == pos_idx;

    return QC_BOOL2TRIAL(res);
}

QC_MKTEST_FUNC(elem);
QC_MKTEST_FUNC(iter);
QC_MKTEST_FUNC(left_content);
QC_MKTEST_FUNC(len);
QC_MKTEST_FUNC(right_content);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(remove),
        QC_MKID_TEST(elem),
        QC_MKID_TEST(iter),
        QC_MKID_TEST(left_content),
        QC_MKID_TEST(len),
        QC_MKID_TEST(right_content),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
