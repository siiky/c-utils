#define BS_CFG_IMPLEMENTATION
#include <utils/bs.h>

#include <common.h>

#define QC_MKID_MOD_TEST(FUNC, TEST) \
    QC_MKID(bs, FUNC, TEST, test)

#define QC_MKID_MOD_PROP(FUNC, TEST) \
    QC_MKID(bs, FUNC, TEST, prop)

#define QC_MKID_MOD_ALL(FUNC) \
    QC_MKID_ALL(bs, FUNC)

static enum theft_alloc_res qc_bs_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);

    struct bs * bs = calloc(1, sizeof(struct bs));
    if (bs == NULL)
        return THEFT_ALLOC_SKIP;

    unsigned nbits = (unsigned) theft_random_bits(t, 8);
    if (!bs_init(bs, nbits))
        return free(bs), THEFT_ALLOC_SKIP;

    for (unsigned segidx = 0; segidx < bs_nbits2arrlen(nbits); segidx++)
        bs->segs[segidx] = theft_random_bits(t, bs_bits_per_seg);

    *output = bs;
    return THEFT_ALLOC_OK;
}

static void qc_bs_free (void * instance, void * env)
{
    UNUSED(env);
    struct bs * bs = instance;
    bs_free(bs);
    free(bs);
}

static void qc_bs_print (FILE * f, const void * instance, void * env)
{
    UNUSED(env);
    const struct bs * bs = instance;
    fprintf(f,
            "{\n"
            "  .segs = \"");
    bs_fprint(bs, f);
    fprintf(f,
            "\",\n"
            "  .nbits = %u,\n"
            "}\n",
            bs->nbits);
}

const struct theft_type_info qc_bs_info = {
    .alloc = qc_bs_alloc,
    .free  = qc_bs_free,
    .print = qc_bs_print,
};
const size_t bs_seg_size = sizeof(segment);
