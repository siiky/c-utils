/* ftr - v2018.11.07-0
 *
 * A future implementation inspired by [stb](https://github.com/nothings/stb)
 *
 * The most up to date version of this file can be found at
 * `include/ftr.h` on [siiky/c-utils](https://github.com/siiky/c-utils)
 * Usage examples can be found at `src/ftr` on the link above
 */

#ifndef _FTR_H
#define _FTR_H

#include <stdbool.h>

/**
 * @brief The future type
 */
struct ftr {
    /** Has this future been ran? */
    bool done;

    /** The procedure to run */
    void * (* proc) (void *);

    /** The arguments to pass to the procedure */
    void * args;

    /** The return value of the procedure */
    void * ret;
};

bool   ftr_clean (struct ftr * self);
bool   ftr_delay (struct ftr * self, void * (* proc) (void *), void * args);
void * ftr_force (struct ftr * self);

# ifdef _FTR_H_IMPLEMENTATION

#include <stddef.h>

/**
 * @brief Create a future
 * @param self The future
 * @param proc The procedure to run
 * @param args Argument to pass to @a proc
 * @return `false` if either @a self or @a proc is NULL,
 *         `true` otherwise
 */
bool   ftr_delay (struct ftr * self, void * (* proc) (void *), void * args)
{
    if (self == NULL || proc == NULL)
        return false;
    ftr_clean(self);
    self->done = false;
    self->proc = proc;
    self->args = args;
    return true;
}

/**
 * @brief Force execution of a future
 * @param self The future
 * @return `NULL` if @a self is `NULL`, the return value of the
 *         associated procedure otherwise
 */
void * ftr_force (struct ftr * self)
{
    if (self == NULL)
        return NULL;

    if (!self->done) {
        self->ret = self->proc(self->args);
        self->done = true;
    }

    return self->ret;
}

/**
 * @brief Clean a future
 * @param self The future
 * @return `true`
 */
bool ftr_clean (struct ftr * self)
{
    if (self != NULL)
        *self = (struct ftr) {0};
    return true;
}

# endif /* _FTR_H_IMPLEMENTATION */
#endif /* _FTR_H */

/*==========================================================
 * License
 *==========================================================
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */
