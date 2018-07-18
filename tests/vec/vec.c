#define VEC_CFG_IMPLEMENTATION
#include "vec.h"

#include <unused.h>

static enum theft_alloc_res qc_vec_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);

    struct vec * vec = malloc(sizeof(struct vec));

    if (vec == NULL)
        return THEFT_ALLOC_SKIP;

    size_t cap = (size_t) theft_random_choice(t, 256);
    size_t len = (size_t) theft_random_choice(t, cap + 1);
    if (!vec_with_capacity(vec, cap)) {
        free(vec);
        return THEFT_ALLOC_SKIP;
    }

    for (size_t i = 0; i < len; i++)
        vec->ptr[i] = (int) theft_random_bits(t, 32);

    vec->length = len;

    vec->idx       = (size_t) theft_random_choice(t, cap);
    /* suppress conversion from `uint64_t` to `unsigned char : 1` warning */
    vec->iterating = 0x1 & theft_random_bits(t, 1);
    vec->reverse   = 0x1 & theft_random_bits(t, 1);

    *output = vec;
    return THEFT_ALLOC_OK;
}

static void qc_vec_free (void * instance, void * env)
{
    UNUSED(env);
    struct vec * vec = (struct vec *) instance;
    *vec = vec_free(*vec);
}

static void qc_vec_print (FILE * f, const void * instance, void * env)
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

struct vec * qc_vec_dup_contents (struct vec * self)
{
    struct vec * ret = malloc(sizeof(struct vec));

    if (ret != NULL) {
        memcpy(ret, self, sizeof(struct vec));

        size_t nbytes = self->capacity * sizeof(int);
        ret->ptr = malloc(nbytes);

        if (ret->ptr != NULL) {
            memcpy(ret->ptr, self->ptr, nbytes);
        } else {
            free(ret);
            ret = NULL;
        }
    }

    return ret;
}

bool qc_vec_search (struct vec * self, int elem, size_t * _i)
{
    const size_t len = self->length;

    for (size_t i = 0; i < len; i++) {
        if (self->ptr[i] == elem) {
            *_i = i;
            return true;
        }
    }

    *_i = len;
    return false;
}
