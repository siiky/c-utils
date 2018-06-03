#ifndef _CHAR_VEC_H
#define _CHAR_VEC_H

char char_dtor (char elem);

#define VEC_DTOR char_dtor
#define VEC_DATA_TYPE char
#define VEC_PREFIX char_
#include <vec.h>

struct char_vec * char_vec_init  (size_t capacity);
void              char_vec_print (struct char_vec * vec);

#endif /* _CHAR_VEC_H */
