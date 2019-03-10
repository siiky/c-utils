#ifndef _CHAR_VEC_H
#define _CHAR_VEC_H

char char_dtor (char elem);

#define VEC_CFG_DTOR char_dtor
#define VEC_CFG_DATA_TYPE char
#define VEC_CFG_VEC cvec
#include <vec.h>

bool cvec_init  (struct cvec * self, size_t capacity);
void cvec_print (struct cvec * vec);

#endif /* _CHAR_VEC_H */
