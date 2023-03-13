#ifndef _INT_VEC_H
#define _INT_VEC_H

#define VEC_CFG_DATA_TYPE int
#define VEC_CFG_VEC ivec
#define VEC_CFG_DATA_TYPE_CMP(L, R) \
  (((L) < (R)) ? \
   1 :           \
   ((L) > (R)) ? \
   -1 :          \
   0)

#include <utils/vec.h>

bool ivec_init  (struct ivec * self, size_t capacity);
void ivec_print (struct ivec * vec);

#endif /* _INT_VEC_H */
