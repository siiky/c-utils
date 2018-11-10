#ifndef _STRM_H
#define _STRM_H

#include <stdbool.h>

struct strm_func {
    enum {
        STRM_FUNC_FILTER,
        STRM_FUNC_MAP,
    } type;

    union {
        bool   (* filter) (void *);
        void * (* map)    (void *);
    } func;
};

#define VEC_CFG_DATA_TYPE struct strm_func
#include "vec.h"

#include "ftr.h"

struct strm {
    void *     head;
    void *     head_pre;
    struct ftr ftr;
    struct vec pipeline;
};

bool   strm_filter   (struct strm * self, bool (* filter) (void *));
bool   strm_free     (struct strm * self);
bool   strm_init     (struct strm * self, void * (* mknext) (void *), void * head_pre);
bool   strm_map      (struct strm * self, void * (* map) (void *));
bool   strm_next     (struct strm * self);
bool   strm_shrink   (struct strm * self);
void * strm_head     (const struct strm * self);
void * strm_head_pre (const struct strm * self);

#endif /* _STRM_H */
