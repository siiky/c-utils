#include <theft.h>

#include <utils/unused.h>

enum theft_alloc_res qc_bool_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);

    bool * x = malloc(sizeof(bool));
    if (x == NULL)
        return THEFT_ALLOC_SKIP;

    *x = (bool) theft_random_bits(t, 1);
    *output = x;

    return THEFT_ALLOC_OK;
}

void qc_bool_free (void * instance, void * env)
{
    UNUSED(env);
    free(instance);
}

void qc_bool_print (FILE * f, const void * instance, void * env)
{
    UNUSED(env);
    bool x = * (const bool *) instance;
    fputs((x) ? "true" : "false", f);
}

const struct theft_type_info qc_bool_info = {
    .alloc = qc_bool_alloc,
    .free  = qc_bool_free,
    .print = qc_bool_print,
};
