#ifndef _CHAR_VEC_H
#define _CHAR_VEC_H

char char_dtor (char elem);

#define VEC_CFG_DTOR char_dtor
#define VEC_CFG_DATA_TYPE char
#define VEC_CFG_PREFIX char_
#include <vec.h>

bool char_vec_init  (struct char_vec * self, size_t capacity);
void char_vec_print (struct char_vec * vec);

#endif /* _CHAR_VEC_H */
