#define VEC_CFG_IMPLEMENTATION
#include "vec.h"

#include <unused.h>

enum theft_alloc_res qc_vec_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);

    struct vec * vec = malloc(sizeof(struct vec));

    if (vec == NULL)
        return THEFT_ALLOC_SKIP;

    size_t cap = (size_t) theft_random_bits(t, 8);
    size_t len = (size_t) ((cap > 0) ?
            ((size_t) theft_random_bits(t, 8)) % cap:
            0);

    if (!vec_with_capacity(vec, cap)) {
        free(vec);
        return THEFT_ALLOC_SKIP;
    }

    for (size_t i = 0; i < len; i++) {
        unsigned int elem = (unsigned int) theft_random_bits(t, 32);
        vec_push(vec, elem);
    }

    *output = vec;
    return THEFT_ALLOC_OK;
}

void qc_vec_free (void * instance, void * env)
{
    UNUSED(env);
    struct vec * vec = (struct vec *) instance;
    *vec = vec_free(*vec);
}

void qc_vec_print (FILE * f, const void * instance, void * env)
{
    UNUSED(env);

    const struct vec * vec = (struct vec *) instance;

    size_t len = vec_len(vec);

    fputc('[', f);

    if (!vec_is_empty(vec)) {
        fprintf(f, " %d", vec_get_nth(vec, 0));

        for (size_t i = 1; i < len; i++) {
            fprintf(f, ", %d", vec_get_nth(vec, i));
        }
    }

    fprintf(f, " ]");
}

const struct theft_type_info qc_vec_info = {
    .alloc = qc_vec_alloc,
    .free  = qc_vec_free,
    .print = qc_vec_print,
};
