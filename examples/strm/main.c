#include "strm.h"

#include <stdio.h>
#include <stdlib.h>

static void * succ (void * _n)
{
    size_t n = (size_t) _n;
    n++;
    return (void *) n;
}

static void * multby (void * _n, size_t by)
{
    size_t n = (size_t) _n;
    n *= by;
    return (void *) n;
}

static void * mult2 (void * n)
{
    return multby(n, 2);
}

static void * mult3 (void * n)
{
    return multby(n, 3);
}

static bool is_even (void * _n)
{
    size_t n = (size_t) _n;
    return (n & 0x1) ^ 0x1;
}

int main (void)
{
    /* map (*2) . filter even . map (*3) $ [0..20] */
    {
        puts("map (*2) . filter even . map (*3) $ [0..20]");
        struct strm strm = {0};

        if (false
                || !strm_init(&strm, succ, (void *) 0) /* enumerate from 0 */
                || !strm_map(&strm, mult3)
                || !strm_filter(&strm, is_even)
                || !strm_map(&strm, mult2)
                || !strm_shrink(&strm)
           ) return EXIT_FAILURE;

        for (strm_next(&strm); /* create the first elem in the stream */
                (size_t) strm_head_pre(&strm) <= 20; /* enumerate up to 20 */
                strm_next(&strm))
            printf("6 * %zu\t=\t%zu\n", (size_t) strm_head_pre(&strm), (size_t) strm_head(&strm));

        strm_free(&strm);
    }

    /*
     * zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
     * zipWith (+) strm1 strm2
     */
    {
        puts("zipWith (+) (map (*2) . filter even . map (*3) $ [0..20]) (map (*2) . filter even . map (*3) $ [1..])");
        struct strm strm1 = {0};
        struct strm strm2 = {0};

        if (false
                /* strm1: map (*2) . filter even . map (*3) $ [0..20] */
                || !strm_init(&strm1, succ, (void *) 0) /* enumerate from 0 */
                || !strm_map(&strm1, mult3)
                || !strm_filter(&strm1, is_even)
                || !strm_map(&strm1, mult2)
                || !strm_shrink(&strm1)
                /* strm2: map (*2) . filter even . map (*3) $ [1..] */
                || !strm_init(&strm2, succ, (void *) 1) /* enumerate from 1 */
                || !strm_map(&strm2, mult3)
                || !strm_filter(&strm2, is_even)
                || !strm_map(&strm2, mult2)
                || !strm_shrink(&strm2)
           ) return EXIT_FAILURE;

        for (strm_next(&strm1), strm_next(&strm2);
                (size_t) strm_head_pre(&strm1) <= 20;
                strm_next(&strm1), strm_next(&strm2))
            printf("%zu\t+\t%zu\t=\t%zu\n",
                    (size_t) strm_head(&strm1), /* :: a */
                    (size_t) strm_head(&strm2), /* :: b */
                    (size_t) strm_head(&strm1) + (size_t) strm_head(&strm2)); /* :: c */

        strm_free(&strm1);
        strm_free(&strm2);
    }

    return EXIT_SUCCESS;
}
