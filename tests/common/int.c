#include <theft.h>

#include "int.h"

#include <unused.h>

enum theft_alloc_res qc_int_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);

    int * x = malloc(sizeof(int));

    if (x == NULL)
        return THEFT_ALLOC_SKIP;

    *x = (int) theft_random_bits(t, 32);

    *output = x;

    return THEFT_ALLOC_OK;
}

void qc_int_free (void * instance, void * env)
{
    UNUSED(env);
    free(instance);
}

void qc_int_print (FILE * f, const void * instance, void * env)
{
    UNUSED(env);

    int x = * (int *) instance;

    fprintf(f, "%d", x);
}

const struct theft_type_info qc_int_info = {
    .alloc = qc_int_alloc,
    .free  = qc_int_free,
    .print = qc_int_print,
};
