#ifndef _INT_VEC_H
#define _INT_VEC_H

#define VEC_DATA_TYPE int
#define VEC_PREFIX int_
#include <vec.h>

struct int_vec * int_vec_init  (size_t capacity);
void             int_vec_print (struct int_vec * vec);

#endif /* _INT_VEC_H */
