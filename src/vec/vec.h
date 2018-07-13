#ifndef _VEC_H
#define _VEC_H

#include "vec/cvec.h"
#include "vec/ivec.h"

#define vec_free(vec) _Generic((vec), \
        struct cvec : cvec_free,      \
        struct ivec : ivec_free       \
        )(vec)

#define vec_print(vec) _Generic((vec), \
        struct cvec * : cvec_print,    \
        struct ivec * : ivec_print     \
        )(vec)

#define vec_init(vec, size) _Generic((vec), \
        struct cvec * : cvec_init,          \
        struct ivec * : ivec_init           \
        )((vec), (size))

#define vec_capacity(vec) _Generic((vec), \
        struct cvec * : cvec_capacity,    \
        struct ivec * : ivec_capacity     \
        )(vec)

#define vec_len(vec) _Generic((vec), \
        struct cvec * : cvec_len,    \
        struct ivec * : ivec_len     \
        )(vec)

#define vec_is_empty(vec) _Generic((vec), \
        struct cvec * : cvec_is_empty,    \
        struct ivec * : ivec_is_empty     \
        )(vec)

#define vec_remove(vec, elem) _Generic((vec), \
        struct cvec * : cvec_remove,          \
        struct ivec * : ivec_remove           \
        )((vec), (elem))

#define vec_swap_remove(vec, elem) _Generic((vec), \
        struct cvec * : cvec_swap_remove,          \
        struct ivec * : ivec_swap_remove           \
        )((vec), (elem))

#endif /* _VEC_H */
