#define VEC_CFG_DATA_TYPE_CMP(L, R) \
    (((void) (L)), ((void) (R)), 0)
#define VEC_CFG_IMPLEMENTATION
#define FTR_CFG_IMPLEMENTATION
#include "strm.h"

#include <assert.h>

/*==========================================================
 * Private functions
 =========================================================*/

/**
 * @brief Clean a stream
 * @param self The stream
 * @returns `true`
 */
static bool strm__clean (struct strm * self)
{
    *self = (struct strm) {0};
    return true;
}

/**
 * @brief Pass the next element through the pipeline
 * @param self The stream
 * @returns `true` if everything went well and the element comming
 *          out the pipeline is valid (i.e., not filtered)
 */
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

/**
 * @brief Add a new pipe to the end of the pipeline
 * @param self The stream
 * @param func The pipe to add to the pipeline
 * @returns `true` if the pipe was successfully added to the pipeline
 */
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

/**
 * @brief Add a filter to a stream's pipeline
 * @param self The stream
 * @param filter The filter pipe to add
 * @returns `true` if the pipe was successfully added to the pipeline
 */
bool strm_filter (struct strm * self, bool (* filter) (void *))
{
    return strm__pipeline_push(self, (struct strm_func) {
            .type = STRM_FUNC_FILTER,
            .func.filter = filter,
            });
}

/**
 * @brief Free a stream
 * @param self The stream
 * @returns `false` if @a self is `NULL`, `true` otherwise
 */
bool strm_free (struct strm * self)
{
    if (self == NULL)
        return false;
    self->pipeline = vec_free(self->pipeline);
    return strm__clean(self);
}

/**
 * @brief Initialize a stream
 * @param self The stream
 * @param mknext The generator function (how to get the next element)
 * @param head_pre The starting element
 * @returns `true` if it successfully initialized the stream
 */
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

/**
 * @brief Add a map to a stream's pipeline
 * @param self The stream
 * @param filter The map pipe to add
 * @returns `true` if the pipe was successfully added to the pipeline
 */
bool strm_map (struct strm * self, void * (* map) (void *))
{
    return strm__pipeline_push(self, (struct strm_func) {
            .type = STRM_FUNC_MAP,
            .func.map = map,
            });
}

/**
 * @brief Get the next element in the stream
 * @param self The stream
 * @returns `false` if @a self is `NULL`, `true` otherwise
 */
bool strm_next (struct strm * self)
{
    if (self == NULL)
        return false;
    while (!strm__next(self));
    return true;
}

/**
 * @brief Minimize memory used by the pipeline
 * @param self The stream
 * @returns `true` if the operation was successful
 */
bool strm_shrink (struct strm * self)
{
    return vec_shrink_to_fit(&self->pipeline);
}

/**
 * @brief Get the head of the stream (after passing through the
 *        pipeline)
 * @param self The stream
 * @returns `NULL` if @a self is `NULL`, the head of the stream
 *          otherwise
 */
void * strm_head (const struct strm * self)
{
    return (self != NULL) ?
        self->head:
        NULL;
}

/**
 * @brief Get the element used to generate the head of the stream
 *        (before passing through the pipeline)
 * @param self The stream
 * @returns `NULL` if @a self is `NULL`, the element used to generate
 *          the head of the stream otherwise
 */
void * strm_head_pre (const struct strm * self)
{
    return (self != NULL) ?
        self->head_pre:
        NULL;
}
