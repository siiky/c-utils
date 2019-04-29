#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(free_range, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(free_range, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop3,                \
            QC_MKID_PROP(TEST),   \
            &qc_vec_info,         \
            &qc_size_t_info,      \
            &qc_size_t_info)

/* Transform the randomly generated (possibly invalid) input in valid input */
#define _QC_PRE()                                \
    do {                                         \
        size_t len = vec->length;                \
        if (len == 0) return THEFT_TRIAL_SKIP;   \
        if (from > len) {                        \
            from = from % len;                   \
            QC_ARG2VAL(2, size_t) = from;        \
        }                                        \
        if (to > len) {                          \
            to = to % len;                       \
            QC_ARG2VAL(3, size_t) = to;          \
        }                                        \
        if (from == to) return THEFT_TRIAL_SKIP; \
        if (from > to) {                         \
            size_t tmp = from;                   \
            from = to;                           \
            to = tmp;                            \
            QC_ARG2VAL(2, size_t) = from;        \
            QC_ARG2VAL(3, size_t) = to;          \
        }                                        \
    } while (0)

static enum theft_trial_res QC_MKID_PROP(meta) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    _QC_PRE();

    bool pre_iterating = vec->iterating;
    bool pre_reverse = vec->reverse;
    size_t pre_cap = vec->capacity;
    size_t pre_idx = vec->idx;
    size_t pre_len = vec->length;

    vec_free_range(vec, from, to);

    bool pos_iterating = vec->iterating;
    bool pos_reverse = vec->reverse;
    size_t pos_cap = vec->capacity;
    size_t pos_idx = vec->idx;
    size_t pos_len = vec->length;

    bool res = true
        && pre_cap       == pos_cap
        && pre_idx       == pos_idx
        && pre_iterating == pos_iterating
        && pre_len       == pos_len
        && pre_reverse   == pos_reverse;

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res QC_MKID_PROP(left_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    _QC_PRE();

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_free_range(vec, from, to);

    bool ret = memcmp(vec->ptr, dup.ptr, from * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(right_content) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    _QC_PRE();

    struct vec dup = {0};
    if (!qc_vec_dup_contents(vec, &dup))
        return THEFT_TRIAL_SKIP;

    vec_free_range(vec, from, to);

    bool ret = memcmp(vec->ptr + to, dup.ptr + to, (dup.length - to) * sizeof(int)) == 0;

    qc_vec_dup_free(&dup);

    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1, void * arg2, void * arg3)
{
    UNUSED(t);

    struct vec * vec = arg1;
    QC_ARG2VAR(2, size_t, from);
    QC_ARG2VAR(3, size_t, to);

    _QC_PRE();

    return QC_BOOL2TRIAL(vec_free_range(vec, from, to));
}

QC_MKTEST_FUNC(left_content);
QC_MKTEST_FUNC(meta);
QC_MKTEST_FUNC(res);
QC_MKTEST_FUNC(right_content);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(free_range),
        QC_MKID_TEST(left_content),
        QC_MKID_TEST(meta),
        QC_MKID_TEST(res),
        QC_MKID_TEST(right_content),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
#undef _QC_PRE
