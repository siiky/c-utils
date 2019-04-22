#include <theft.h>

#include <utils/unused.h>

enum theft_alloc_res qc_void_ptr_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(t);
    UNUSED(env);

    *output = output;

    return THEFT_ALLOC_OK;
}

void qc_void_ptr_free (void * instance, void * env)
{
    UNUSED(instance);
    UNUSED(env);
}

void qc_void_ptr_print (FILE * f, const void * instance, void * env)
{
    UNUSED(env);
    fprintf(f, "%p", instance);
}

const struct theft_type_info qc_void_ptr_info = {
    .alloc = qc_void_ptr_alloc,
    .free  = qc_void_ptr_free,
    .print = qc_void_ptr_print,
};
