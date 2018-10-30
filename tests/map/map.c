#define MAP_CFG_IMPLEMENTATION
#include "map.h"

static enum theft_alloc_res qc_map_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(t);
    UNUSED(env);
    UNUSED(output);
    return THEFT_ALLOC_ERROR;
}

static void qc_map_free (void * instance, void * env)
{
    UNUSED(instance);
    UNUSED(env);
}

static void qc_map_print (FILE * f, const void * instance, void * env)
{
    UNUSED(f);
    UNUSED(instance);
    UNUSED(env);
}

const struct theft_type_info qc_map_info = {
    .alloc = qc_map_alloc,
    .free  = qc_map_free,
    .print = qc_map_print,
};

unsigned int qc_map_int_hash (const int k)
{
    return (unsigned int) k;
}

int qc_map_int_cmp (const int k1, const int k2)
{
    return (k1 < k2) ?
        -1 :
        (k1 > k2) ?
        1 :
        0;
}
