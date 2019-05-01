#define QC_MKID_PROP(TEST) \
    QC_MKID_MOD_PROP(cmp, TEST)

#define QC_MKID_TEST(TEST) \
    QC_MKID_MOD_TEST(cmp, TEST)

#define QC_MKTEST_FUNC(TEST)      \
    QC_MKTEST(QC_MKID_TEST(TEST), \
            prop2,                \
            QC_MKID_PROP(TEST),   \
            &qc_bs_info,          \
            &qc_bs_info)

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    const struct bs * bs = arg1;
    const struct bs * other = arg2;
    struct bs clone[1];
    struct bs other_clone[1];
    if (!bs_clone(bs, clone) || !bs_clone(other, other_clone))
        return THEFT_TRIAL_SKIP;
    bs_cmp(bs, other);
    size_t bs_len = bs_nbits2arrlen(bs->nbits);
    size_t other_len = bs_nbits2arrlen(other->nbits);
    bool res = true
        && memcmp(bs->segs, clone->segs, bs_len * sizeof(segment)) == 0
        && memcmp(other->segs, other_clone->segs, other_len * sizeof(segment)) == 0;
    bs_free(clone);
    bs_free(other_clone);
    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    const struct bs * bs = arg1;
    const struct bs * other = arg2;
    size_t mlen = (bs->nbits < other->nbits) ?
        bs_nbits2arrlen(bs->nbits):
        bs_nbits2arrlen(other->nbits);
    int res = bs_cmp(bs, other);
    bool ret = (bs->nbits < other->nbits) ?
        (res < 0):
        (bs->nbits > other->nbits) ?
        (res > 0):
        (res == 0) ?
        (memcmp(bs->segs, other->segs, mlen * sizeof(segment)) == 0):
        true;
    return QC_BOOL2TRIAL(ret);
}

static enum theft_trial_res QC_MKID_PROP(res_eq) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);
    const struct bs * bs = arg1;
    struct bs * other = arg2;
    if (!bs_free(other) || !bs_clone(bs, other))
        return THEFT_TRIAL_SKIP;
    bool ret = bs_cmp(bs, other) == 0;
    return QC_BOOL2TRIAL(ret);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(res);
QC_MKTEST_FUNC(res_eq);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(cmp),
        QC_MKID_TEST(content),
        QC_MKID_TEST(res),
        QC_MKID_TEST(res_eq),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
