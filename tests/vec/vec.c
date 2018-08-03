#define VEC_CFG_IMPLEMENTATION
#include "vec.h"

#include <assert.h>
#include <unused.h>

static enum theft_alloc_res qc_vec_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);

    struct vec * vec = malloc(sizeof(struct vec));
    if (vec == NULL)
        return THEFT_ALLOC_SKIP;

    vec__clean(vec);

    size_t cap = (size_t) theft_random_choice(t, 1024);

    if (cap > 0)
        if (!vec_with_capacity(vec, cap))
            return free(vec), THEFT_ALLOC_SKIP;

    size_t len = (size_t) theft_random_choice(t, cap + 1);
    assert(len <= cap);

    for (size_t i = 0; i < len; i++)
        vec->ptr[i] = (int) theft_random_bits(t, 32);

    vec->length = len;

    vec->idx       = (size_t) theft_random_choice(t, len);
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

    const struct vec * vec = instance;

    fprintf(f,
            "{\n"
            "  .length    = %zu,\n"
            "  .capacity  = %zu,\n"
            "  .idx       = %zu,\n"
            "  .iterating = %hu,\n"
            "  .reverse   = %hu,\n"
            "  .ptr       = { ",
            vec->length,
            vec->capacity,
            vec->idx,
            vec->iterating,
            vec->reverse
           );

    if (!vec_is_empty(vec)) {
        size_t len = vec_len(vec);
        for (size_t i = 0; i < len; i++)
            fprintf(f, "%d, ", vec_get_nth(vec, i));
    }

    fprintf(f, "},\n}");
}

const struct theft_type_info qc_vec_info = {
    .alloc = qc_vec_alloc,
    .free  = qc_vec_free,
    .print = qc_vec_print,
};

bool qc_vec_dup_contents (const struct vec * self, struct vec * dup)
{
    bool ret = true;

    memcpy(dup, self, sizeof(struct vec));

    size_t nbytes = self->capacity * sizeof(int);
    dup->ptr = malloc(nbytes);

    if (dup->ptr != NULL)
        memcpy(dup->ptr, self->ptr, nbytes);
    else
        ret = false;

    return ret;
}

void qc_vec_dup_free (struct vec * self)
{
    *self = vec_free(*self);
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

size_t qc_vec_count (struct vec * self, bool pred (const int *))
{
    size_t ret = 0;

    const size_t len = self->length;
    for (size_t i = 0; i < len; i++)
        if (pred(self->ptr + i))
            ret++;

    return ret;
}
