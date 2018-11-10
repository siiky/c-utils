#define VEC_CFG_DATA_TYPE_CMP(L, R) \
    (((void) (L)), ((void) (R)), 0)
#define VEC_CFG_IMPLEMENTATION
#define _FTR_H_IMPLEMENTATION
#include "strm.h"

#include <assert.h>

/*==========================================================
 * Private functions
 =========================================================*/

static bool strm__clean (struct strm * self)
{
    *self = (struct strm) {0};
    return true;
}

static bool strm__next (struct strm * self)
{
    bool ret = true;
    self->head_pre = self->head = ftr_force(&self->ftr);
    ftr_delay(&self->ftr, self->ftr.proc, self->head_pre);

    for (vec_iter(&self->pipeline);
            ret && vec_itering(&self->pipeline);
            vec_iter_next(&self->pipeline)) {
        struct strm_func f = vec_get_nth(&self->pipeline, vec_iter_idx(&self->pipeline));
        assert(f.type == STRM_FUNC_MAP || f.type == STRM_FUNC_FILTER);

        switch (f.type) {
            case STRM_FUNC_MAP:
                self->head = f.func.map(self->head);
                break;
            case STRM_FUNC_FILTER:
                ret = f.func.filter(self->head);
                break;
            default:
                ret = false;
        }
    }

    vec_iter_end(&self->pipeline);

    return ret;
}

static bool strm__pipeline_push (struct strm * self, struct strm_func func)
{
    return true
        && self != NULL
        && func.func.map != NULL
        && vec_push(&self->pipeline, func);
}

/*==========================================================
 * Public functions
 =========================================================*/

bool strm_filter (struct strm * self, bool (* filter) (void *))
{
    return strm__pipeline_push(self, (struct strm_func) {
            .type = STRM_FUNC_FILTER,
            .func.filter = filter,
            });
}

bool strm_free (struct strm * self)
{
    if (self == NULL)
        return false;
    self->pipeline = vec_free(self->pipeline);
    return strm__clean(self);
}

bool strm_init (struct strm * self, void * (* mknext) (void *), void * head_pre)
{
    return true
        && self != NULL
        && mknext != NULL
        && strm__clean(self)
        && ftr_delay(&self->ftr, mknext, head_pre)
        && (self->ftr.done = true)
        && (self->ftr.ret  = head_pre, true)
        && (self->head_pre = head_pre, true);
}

bool strm_map (struct strm * self, void * (* map) (void *))
{
    return strm__pipeline_push(self, (struct strm_func) {
            .type = STRM_FUNC_MAP,
            .func.map = map,
            });
}

bool strm_next (struct strm * self)
{
    if (self == NULL)
        return false;

    while (!strm__next(self));
    return true;
}

bool strm_shrink (struct strm * self)
{
    return vec_shrink_to_fit(&self->pipeline);
}

void * strm_head (const struct strm * self)
{
    return (self != NULL) ?
        self->head:
        NULL;
}

void * strm_head_pre (const struct strm * self)
{
    return (self != NULL) ?
        self->head_pre:
        NULL;
}
