#include <stddef.h>

#define FTR_CFG_ARGS_TYPE size_t
#define FTR_CFG_RET_TYPE size_t
#define FTR_CFG_FTR my_ftr
#define _FTR_H_IMPLEMENTATION
#include <utils/ftr.h>

#include <unistd.h>

#include <stdio.h>

size_t sleep_secs (size_t secs)
{
    sleep((unsigned) secs);
    return secs;
}

#define NFTR 10
int main (void)
{
    struct my_ftr ftr[NFTR] = {0};
    for (unsigned i = 0; i < NFTR; i++)
        my_ftr_clean(ftr + i);

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Creating future #%u\n", i);
        my_ftr_delay(ftr + i,
                sleep_secs,
                NFTR - i);
    }

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Forcing future #%u... ", i);
        fflush(stdout);
        printf("returned %zu\n", my_ftr_force(ftr + i));
    }

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Forcing future #%u again... ", i);
        fflush(stdout);
        printf("returned %zu\n", my_ftr_force(ftr + i));
    }

    return 0;
}
