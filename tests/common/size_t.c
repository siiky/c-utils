#include <theft.h>

#include "size_t.h"

#include <unused.h>

enum theft_alloc_res qc_size_t_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);

    size_t * x = malloc(sizeof(size_t));

    if (x == NULL)
        return THEFT_ALLOC_SKIP;

    *x = (size_t) theft_random_bits(t, 32);

    *output = x;

    return THEFT_ALLOC_OK;
}

void qc_size_t_free (void * instance, void * env)
{
    UNUSED(env);
    free(instance);
}

void qc_size_t_print (FILE * f, const void * instance, void * env)
{
    UNUSED(env);

    size_t x = * (size_t *) instance;

    fprintf(f, "%zu", x);
}

const struct theft_type_info qc_size_t_info = {
    .alloc = qc_size_t_alloc,
    .free  = qc_size_t_free,
    .print = qc_size_t_print,
};
