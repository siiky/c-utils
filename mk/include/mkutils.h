#ifndef _MKUTILS_H
#define _MKUTILS_H

#define ABORT(L, C, ...)              \
    if (C) {                          \
        fprintf(stderr, __VA_ARGS__); \
        goto L;                       \
    }

#define IFNOTNULL(V, F)         if ((V) != NULL) F(V);
#define file_exists(F)          (access(F, F_OK) == 0)

#endif /* _MKUTILS_H */
