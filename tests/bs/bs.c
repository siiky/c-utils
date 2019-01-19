#define BS_CFG_IMPLEMENTATION
#include "bs.h"

static enum theft_alloc_res qc_bs_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);

    struct bs * bs = malloc(sizeof(struct bs));
    if (bs == NULL)
        return THEFT_ALLOC_SKIP;


    *bs = (struct bs) {0};

    unsigned nbits = (unsigned) theft_random_bits(t, 16);

    if (!bs_init(bs, nbits))
        return free(bs), THEFT_ALLOC_SKIP;

    for (unsigned n = 0; n < nbits; n++)
        bs_set(bs, n, 0x1 & theft_random_bits(t, 1));

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
            "  .bytes = %p,\n"
            "  .nbits = %u,\n"
            "}\n",
            bs->bytes,
            bs->nbits);
}

const struct theft_type_info qc_bs_info = {
    .alloc = qc_bs_alloc,
    .free  = qc_bs_free,
    .print = qc_bs_print,
};
const size_t bs_byte_size = sizeof(struct byte);