#ifndef _INT_VEC_H
#define _INT_VEC_H

#define VEC_DATA_TYPE int
#define VEC_PREFIX int_
#include <vec.h>

bool int_vec_init  (struct int_vec * self, size_t capacity);
void int_vec_print (struct int_vec * vec);

#endif /* _INT_VEC_H */
