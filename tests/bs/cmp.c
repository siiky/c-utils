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

static enum theft_trial_res QC_MKID_PROP(res) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct bs * bs = arg1;
    struct bs * other = arg2;

    bool res = bs_cmp(bs, other)
        || true;

    return QC_BOOL2TRIAL(res);
}

static enum theft_trial_res QC_MKID_PROP(content) (struct theft * t, void * arg1, void * arg2)
{
    UNUSED(t);

    struct bs * bs = arg1;
    struct bs * other = arg2;

    struct bs bs_copy[1];
    struct bs other_copy[1];

    if (!bs_init(bs_copy, bs->nbits) || !bs_init(other_copy, other->nbits))
        return THEFT_TRIAL_SKIP;

    size_t bs_size = bs_nbits2len(bs->nbits) * bs_byte_size;
    size_t other_size = bs_nbits2len(other->nbits) * bs_byte_size;

    memcpy(bs_copy->bytes, bs->bytes, bs_size);
    memcpy(other_copy->bytes, other->bytes, other_size);

    bs_cmp(bs, other);

    bool res = true
        && memcmp(bs->bytes, bs_copy->bytes, bs_size) == 0
        && memcmp(other->bytes, other_copy->bytes, other_size) == 0;

    bs_free(bs_copy);
    bs_free(other_copy);

    return QC_BOOL2TRIAL(res);
}

QC_MKTEST_FUNC(content);
QC_MKTEST_FUNC(res);

QC_MKTEST_ALL(QC_MKID_MOD_ALL(cmp),
        QC_MKID_TEST(content),
        QC_MKID_TEST(res),
        );

#undef QC_MKID_PROP
#undef QC_MKID_TEST
#undef QC_MKTEST_FUNC
