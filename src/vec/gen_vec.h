#ifndef _GEN_VEC_H
#define _GEN_VEC_H

#include "char_vec.h"
#include "int_vec.h"

#define vec_free(vec) _Generic((vec), \
        struct char_vec : char_free,  \
        struct int_vec  : int_free    \
        )(vec)

#define vec_print(vec) _Generic((vec),      \
        struct char_vec * : char_vec_print, \
        struct int_vec *  : int_vec_print   \
        )(vec)

#define vec_init(vec, size) _Generic((vec), \
        struct char_vec * : char_vec_init,  \
        struct int_vec *  : int_vec_init    \
        )((vec), (size))

#define vec_capacity(vec) _Generic((vec),  \
        struct char_vec * : char_capacity, \
        struct int_vec *  : int_capacity   \
        )(vec)

#define vec_len(vec) _Generic((vec),  \
        struct char_vec * : char_len, \
        struct int_vec *  : int_len   \
        )(vec)

#endif /* _GEN_VEC_H */
