#ifndef _INT_VEC_H
#define _INT_VEC_H

#define VEC_CFG_DATA_TYPE int
#define VEC_CFG_VEC ivec
#include <vec.h>

bool ivec_init  (struct ivec * self, size_t capacity);
void ivec_print (struct ivec * vec);

#endif /* _INT_VEC_H */
