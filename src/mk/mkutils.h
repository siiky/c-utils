#ifndef _MKUTILS_H
#define _MKUTILS_H

#include <utils/ifnotnull.h>
#include <utils/ifjmp.h>

#define ABORT(L, C, ...)              \
    if (C) {                          \
        fprintf(stderr, __VA_ARGS__); \
        goto L;                       \
    }

#define file_exists(F)          (access(F, F_OK) == 0)

#endif /* _MKUTILS_H */
