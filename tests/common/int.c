#include <theft.h>

#include <utils/unused.h>

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
    int x = * (const int *) instance;
    fprintf(f, "%d", x);
}

const struct theft_type_info qc_int_info = {
    .alloc = qc_int_alloc,
    .free  = qc_int_free,
    .print = qc_int_print,
};

int qc_int_compar (const void * _a, const void * _b)
{
    int a = * (const int *) _a;
    int b = * (const int *) _b;

    return (a > b) ?
        1:
        (a < b) ?
        -1:
        0;
}
