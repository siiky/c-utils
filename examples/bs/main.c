#include <bs.h>

#include <stdlib.h>

#ifndef NDEBUG
#define bool2str(v)          ((v) ? "true" : "false")
#define bs_set(bs, nth, val) (printf("bs_set(bs, %u, %s) -> %s\n", nth, bool2str(val), bool2str(bs_set(bs, nth, val))))
#define bs_flip(bs, nth)     (printf("bs_flip(bs, %u)    -> %s\n", nth,                bool2str(bs_flip(bs, nth))))
#define bs_get(bs, nth)      (printf("bs_get(bs, %u)     -> %s\n", nth,                bool2str(bs_get(bs, nth))))
#endif

int main (int argc, char ** argv)
{
    struct bs bs[1] = {0};
    unsigned nbits = (argc > 1) ?
        (unsigned) atoi(argv[1]):
        1000;

    if (bs_init(bs, nbits)) {
        bs_print(bs);

        for (unsigned i = 0; i < nbits; i++)
            bs_set(bs, i, !(i & 0x1));
        for (unsigned i = 0; i < nbits; i++)
            bs_get(bs, i);
        bs_print(bs);

        for (unsigned i = 0; i < nbits; i++)
            bs_flip(bs, i);
        for (unsigned i = 0; i < nbits; i++)
            bs_get(bs, i);
        bs_print(bs);

        struct bs clone[1] = {0};
        if (bs_clone(bs, clone)) {
            printf("cmp = %d\n", bs_cmp(bs, clone));

            bs_flip(clone, clone->nbits - 1);
            printf("cmp = %d\n", bs_cmp(bs, clone));

            bs_flip(clone, clone->nbits - 1);
            bs_flip(clone, clone->nbits - 2);
            printf("cmp = %d\n", bs_cmp(bs, clone));

            bs_free(clone);
        }

        bs_free(bs);
    }

    return 0;
}
