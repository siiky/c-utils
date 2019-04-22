/* ftr - v2018.12.15-0
 *
 * A future implementation inspired by
 *  * Scheme
 *  * [stb](https://github.com/nothings/stb)
 *
 * The most up to date version of this file can be found at
 * `include/utils/ftr.h` on [siiky/c-utils](https://github.com/siiky/c-utils)
 * More usage examples can be found at `examples/ftr` on the link above
 */

# if 0 /* EXAMPLE */
#include <stddef.h>

// Optionally, define the input argument's type
#define FTR_CFG_ARGS_TYPE size_t

// Optionally, define the associated procedure's output type
#define FTR_CFG_RET_TYPE size_t

// Optionally, define the struct identifier (defaults to `ftr`)
#define FTR_CFG_FTR my_ftr

// Optionally, define a prefix for the generated functions (defaults
// to `FTR_CFG_FTR_`, in this case `my_ftr_`)
#define FTR_CFG_PREFIX my_

// Optionally, define NDEBUG to disable asserts inside ftr.h
//#define NDEBUG

// Create implementation, instead of working as a header
#define _FTR_H_IMPLEMENTATION
#include <ftr.h>

#include <unistd.h>
#include <stdio.h>

size_t sleep_secs (size_t secs)
{
    sleep((unsigned) secs);
    return secs;
}

#define NFTR 10
int main (void)
{
    struct my_ftr ftr[NFTR] = {0};
    for (unsigned i = 0; i < NFTR; i++)
        my_ftr_clean(ftr + i);

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Creating future #%u\n", i);
        my_ftr_delay(ftr + i,
                sleep_secs,
                NFTR - i);
    }

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Forcing future #%u... ", i);
        fflush(stdout);
        printf("returned %zu\n", my_ftr_force(ftr + i));
    }

    for (unsigned i = 0; i < NFTR; i++) {
        printf("Forcing future #%u again... ", i);
        fflush(stdout);
        printf("returned %zu\n", my_ftr_force(ftr + i));
    }

    return 0;
}
# endif /* EXAMPLE */

/*
 * <stdbool.h>
 *  bool
 *  false
 *  true
 */
#include <stdbool.h>

/*
 * Magic from `sort.h`
 */
# define FTR_CFG_CONCAT(A, B)    A ## B
# define FTR_CFG_MAKE_STR1(A, B) FTR_CFG_CONCAT(A, B)
# define FTR_CFG_MAKE_STR(A)     FTR_CFG_MAKE_STR1(FTR_CFG_PREFIX, A)

/*
 * Type of the input argument of the associated procedure
 */
# ifndef FTR_CFG_ARGS_TYPE
#  define FTR_CFG_ARGS_TYPE_DEFAULT
#  define FTR_CFG_ARGS_TYPE void
# endif /* FTR_CFG_ARGS_TYPE */

/*
 * The future name defaults to `ftr`
 */
# ifndef FTR_CFG_FTR
#  define FTR_CFG_FTR ftr
# endif /* FTR_CFG_FTR */

/*
 * The prefix defaults to the future name with an '_' appended
 */
# ifndef FTR_CFG_PREFIX
#  define FTR_CFG_PREFIX FTR_CFG_MAKE_STR1(FTR_CFG_FTR, _)
# endif /* FTR_CFG_PREFIX */

/**
 * @brief The future type
 */
struct FTR_CFG_FTR {
    /** Has this future been ran? */
    bool done;

    /** The procedure to run */
    FTR_CFG_RET_TYPE (* proc) (FTR_CFG_ARGS_TYPE);

    /** The arguments to pass to the procedure */
# ifndef FTR_CFG_ARGS_TYPE_DEFAULT
    FTR_CFG_ARGS_TYPE args;
# endif /* FTR_CFG_ARGS_TYPE_DEFAULT */

    /** The return value of the procedure */
# ifdef FTR_CFG_RET_TYPE
    FTR_CFG_RET_TYPE ret;
# endif /* FTR_CFG_RET_TYPE */
};

/*==========================================================
 * Function names
 *=========================================================*/
#define FTR_CLEAN FTR_CFG_MAKE_STR(clean)
#define FTR_DELAY FTR_CFG_MAKE_STR(delay)
#define FTR_FORCE FTR_CFG_MAKE_STR(force)

/*==========================================================
 * Function prototypes
 *
 * RETURN TYPE   NAME      PARAMETER LIST
 *==========================================================*/
FTR_CFG_RET_TYPE FTR_FORCE (struct FTR_CFG_FTR * self);
bool             FTR_CLEAN (struct FTR_CFG_FTR * self);
# ifndef FTR_CFG_ARGS_TYPE_DEFAULT
bool             FTR_DELAY (struct FTR_CFG_FTR * self, FTR_CFG_RET_TYPE (* proc) (FTR_CFG_ARGS_TYPE), FTR_CFG_ARGS_TYPE);
# else
bool             FTR_DELAY (struct FTR_CFG_FTR * self, FTR_CFG_RET_TYPE (* proc) (FTR_CFG_ARGS_TYPE));
# endif /* FTR_CFG_ARGS_TYPE_DEFAULT */

# ifdef _FTR_H_IMPLEMENTATION

/*
 * <stddef.h>
 *  NULL
 *
 * <assert.h>
 *  assert()
 */
#include <stddef.h>
#include <assert.h>

/**
 * @brief Create a future
 * @param self The future
 * @param proc The procedure to run
 * @param args Argument to pass to @a proc
 * @return `false` if either @a self or @a proc is NULL,
 *         `true` otherwise
 */
# ifndef FTR_CFG_ARGS_TYPE_DEFAULT
bool FTR_DELAY (struct FTR_CFG_FTR * self, FTR_CFG_RET_TYPE (* proc) (FTR_CFG_ARGS_TYPE), FTR_CFG_ARGS_TYPE args)
# else
bool FTR_DELAY (struct FTR_CFG_FTR * self, FTR_CFG_RET_TYPE (* proc) (FTR_CFG_ARGS_TYPE))
# endif /* FTR_CFG_ARGS_TYPE_DEFAULT */
{
    if (self == NULL || proc == NULL)
        return false;
    FTR_CLEAN(self);
    self->done = false;
    self->proc = proc;
# ifndef FTR_CFG_ARGS_TYPE_DEFAULT
    self->args = args;
# endif /* FTR_CFG_ARGS_TYPE_DEFAULT */
    return true;
}

/**
 * @brief Force execution of a future
 * @param self The future
 * @return `NULL` if @a self is `NULL`, the return value of the
 *         associated procedure otherwise
 */
FTR_CFG_RET_TYPE FTR_FORCE (struct FTR_CFG_FTR * self)
{
    assert(self != NULL);

    if (!self->done) {
# ifdef FTR_CFG_RET_TYPE
        self->ret =
# endif /* FTR_CFG_RET_TYPE */

            self->proc(
# ifndef FTR_CFG_ARGS_TYPE_DEFAULT
                    self->args
# endif /* FTR_CFG_ARGS_TYPE_DEFAULT */
                    );

        self->done = true;
    }

# ifdef FTR_CFG_RET_TYPE
    return self->ret;
# endif /* FTR_CFG_RET_TYPE */
}

/**
 * @brief Clean a future
 * @param self The future
 * @return `true`
 */
bool FTR_CLEAN (struct FTR_CFG_FTR * self)
{
    if (self != NULL)
        *self = (struct FTR_CFG_FTR) {0};
    return true;
}

/*==========================================================
 * Implementation clean up
 *=========================================================*/

/*
 * Other
 */
#undef _FTR_H_IMPLEMENTATION

# endif /* _FTR_H_IMPLEMENTATION */

/*==========================================================
 * Clean up
 *=========================================================*/

/*
 * Functions
 */
#undef FTR_CLEAN
#undef FTR_DELAY
#undef FTR_FORCE

/*
 * Other
 */
#undef FTR_CFG_ARGS_TYPE
#undef FTR_CFG_ARGS_TYPE_DEFAULT
#undef FTR_CFG_CONCAT
#undef FTR_CFG_FTR
#undef FTR_CFG_MAKE_STR
#undef FTR_CFG_MAKE_STR1
#undef FTR_CFG_PREFIX
#undef FTR_CFG_RET_TYPE

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
