#define _FTR_H_IMPLEMENTATION
#include "ftr.h"

#include <unistd.h>

#include <stddef.h>
#include <stdio.h>

void * sleep_secs (void * args)
{
    size_t secs = (size_t) args;
    sleep((unsigned) secs);
    return (void *) args;
}

#define NFTR 10
int main (void)
{
    struct ftr ftr[NFTR] = {0};
    for (unsigned i = 0; i < NFTR; i++)
        ftr_clean(ftr + i);

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Creating future #%u\n", i);
        ftr_delay(ftr + i,
                sleep_secs,
                (void *) ((size_t) (NFTR - i)));
    }

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Forcing future #%u\n", i);
        size_t s = (size_t) ftr_force(ftr + i);
        printf("Future #%u returned %zu\n", i, s);
    }

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Forcing future #%u again\n", i);
        size_t s = (size_t) ftr_force(ftr + i);
        printf("Future #%u returned %zu\n", i, s);
    }

    return 0;
}
