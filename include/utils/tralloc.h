/* tralloc - v2019.04.28-0
 *
 * A simple hack to track memory allocations inspired by:
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *
 * The most up to date version of this file can be found at
 * `include/utils/tralloc.h` on [siiky/c-utils](https://git.sr.ht/~siiky/c-utils)
 * More usage examples can be found at `examples/tralloc` on the link above
 *
 * This implementation depends on `vec.h`, which can be found at
 * `include/utils/vec.h`
 */

#if 0
/* Optionally define the allocation functions to use */
#define TRALLOC_CFG_MALLOC malloc
#define TRALLOC_CFG_CALLOC calloc
#define TRALLOC_CFG_REALLOC realloc
#define TRALLOC_CFG_FREE free

/* Enable tracing */
#define TRALLOC_CFG_TRACE

/* Create implementation (do this in a single file) */
#define TRALLOC_CFG_IMPLEMENTATION

/* You have to include <stdlib.h> if using its functions */
#include <stdlib.h>
#include <tralloc.h>
#include <assert.h>

int main (void)
{
    void * ptr = trmalloc(1024);
    ptr = trcalloc(10240, sizeof(int));
    ptr = trrealloc(ptr, 1024);
    trfree(ptr);
    int ret = (trprint() == 0) ? 0 : 1; /* print all unfreed memory */
    trdeinit(); /* free internal state and all unfreed memory */
    assert(trprint());
    return ret;
}
#endif /* EXAMPLE */

# ifndef TRALLOC_CFG_MALLOC
#  define TRALLOC_CFG_MALLOC malloc
# endif /* TRALLOC_CFG_MALLOC */

# ifndef TRALLOC_CFG_CALLOC
#  define TRALLOC_CFG_CALLOC calloc
# endif /* TRALLOC_CFG_CALLOC */

# ifndef TRALLOC_CFG_REALLOC
#  define TRALLOC_CFG_REALLOC realloc
# endif /* TRALLOC_CFG_REALLOC */

# ifndef TRALLOC_CFG_FREE
#  define TRALLOC_CFG_FREE free
# endif /* TRALLOC_CFG_FREE */

#ifndef _TRALLOC_H
#define _TRALLOC_H

/*
 * <stdio.h>
 *  FILE
 *  fprintf()
 *  stderr
 *  stdout
 */
#include <stdio.h>

/*
 * Define TRALLOC_CFG_TRACE to use tralloc
 */
#ifndef TRALLOC_CFG_TRACE

#    define trdeinit() ((void)0)

#    define trcalloc  TRALLOC_CFG_CALLOC
#    define trfree    TRALLOC_CFG_FREE
#    define trmalloc  TRALLOC_CFG_MALLOC
#    define trrealloc TRALLOC_CFG_REALLOC

/* if trace is not enabled, the tr*print() macros work as if there were no leaks */
#    define treprint()     true
#    define trfprint(OUTF) true
#    define trprint()      true

#    define trused() 0UL

#else

#    define trdeinit()            _trdeinit()

#    define trcalloc(nmemb, size) _trcalloc((nmemb), (size), __FILE__, __func__, __LINE__)
#    define trfree(ptr)           _trfree(ptr)
#    define trmalloc(size)        _trmalloc((size), __FILE__, __func__, __LINE__)
#    define trrealloc(ptr, size)  _trrealloc((ptr), (size), __FILE__, __func__, __LINE__)

#    define treprint()            _treprint()
#    define trfprint(OUTF)        _trfprint(OUTF)
#    define trprint()             _trprint()

#    define trused() _trused()

#endif /* TRALLOC_CFG_TRACE */

/**
 * @brief Calls `_trfprint()` with `stderr`
 * @see _trfprint
 */
size_t _treprint (void);

/**
 * @brief If there is unfreed memory, and @a stream is not NULL,
 *        print (to @a stream) each block in the format
 *        "file:function:line address size".
 * @param stream Where to write
 * @returns Unfreed memory (in bytes)
 */
size_t _trfprint (FILE * stream);

/**
 * @brief Calls `_trfprint()` with `stdout`
 * @see _trfprint
 */
size_t _trprint (void);

/**
 * @brief Wrapper for `TRALLOC_CFG_FREE()`. Remove @a ptr from the list
 *        of unfreed memory and free it
 * @param ptr A pointer to dynamically allocated memory
 *
 * If @a ptr is NULL or is not in the list, e.g., if it was obtained
 * with a direct call to (m|c|re)alloc, nothing is done
 */
void _trfree (void * ptr);

/**
 * @brief Wrapper for `TRALLOC_CFG_CALLOC()`. Record where the allocation
 *        occurred (@a file, @a func, @a line), its size (@a nmemb * @a size)
 *        and the address returned by `TRALLOC_CFG_CALLOC()`
 * @param nmemb Number of members
 * @param size Size of each member
 * @param file filename
 * @param func function name
 * @param line Line number
 * @returns Same as `calloc()`
 */
void * _trcalloc (size_t nmemb, size_t size, const char * file, const char * func, unsigned short line);

/**
 * @brief Wrapper for `TRALLOC_CFG_MALLOC()`. Record where the allocation
 *        occurred (@a file, @a func, @a line), its size (@a size)
 *        and the address returned by `TRALLOC_CFG_MALLOC()`
 * @param size Size of each member
 * @param file filename
 * @param func function name
 * @param line Line number
 * @returns Same as `malloc()`
 */
void * _trmalloc (size_t size, const char * file, const char * func, unsigned short line);

/**
 * @brief Wrapper for `TRALLOC_CFG_REALLOC()`. Record where the
 *        allocation occurred (@a file, @a func, @a line), its size (@a size)
 *        and the new address returned by `TRALLOC_CFG_REALLOC()`.
 *        If @a ptr is NULL, `_trmalloc()` is called instead.
 *        If @a ptr is already in the list, it is updated.
 *        If @a size is 0, `_trfree()` is called instead, and NULL is returned.
 * @param ptr Pointer to some dynamically allocated block of memory
 * @param size Size of each member
 * @param file filename
 * @param func function name
 * @param line Line number
 * @returns Same as `realloc()`, except if @a ptr is not NULL and
 *          @a size is 0, then NULL is returned (while `realloc()` may
 *          return either NULL or a pointer passable to `free()`)
 */
void * _trrealloc (void * ptr, size_t size, const char * file, const char * func, unsigned short line);

/**
 * @brief Deinitialize the trace structure (this includes free()ing
 *        unfreed memory)
 */
void _trdeinit (void);

/**
 * @brief Calculate total memory (in Bytes) allocated (and not yet free()'d)
 *        through tralloc
 * @returns Total memory used currently (in Bytes)
 */
size_t _trused (void);

#endif /* _TRALLOC_H */

#ifdef TRALLOC_CFG_IMPLEMENTATION

/**
 * Info of an allocated memory block
 */
struct trs {
    /** file where this block of memory was allocated */
    const char * file;
    /** function where this block of memory was allocated */
    const char * func;
    /** line where this block of memory was allocated */
    unsigned short line;

    /** size of this block of memory */
    size_t size;
    /** pointer to the allocated block of memory */
    void * ptr;
};

/*
 * NOTE: Comparing pointers that are not of the same array/object is
 *       *NOT* guaranteed to work by the C standard
 */
static int trs_cmp (struct trs a, struct trs b)
{
    return (a.ptr < b.ptr) ?
        1:
        (a.ptr > b.ptr) ?
        -1:
        0;
}

static struct trs _trs_free (struct trs self)
{
    if (self.ptr)
        TRALLOC_CFG_FREE(self.ptr);
    return (struct trs) {0};
}

static struct trs _trs_new (size_t size, void * ptr, const char * file, const char * func, unsigned short line)
{
    return (struct trs) {
        .file = file,
        .func = func,
        .line = line,
        .size = size,
        .ptr = ptr,
    };
}

#define VEC_CFG_CALLOC        TRALLOC_CFG_CALLOC
#define VEC_CFG_FREE          TRALLOC_CFG_FREE
#define VEC_CFG_MALLOC        TRALLOC_CFG_MALLOC
#define VEC_CFG_REALLOC       TRALLOC_CFG_REALLOC
#define VEC_CFG_DATA_TYPE     struct trs
#define VEC_CFG_DATA_TYPE_CMP trs_cmp
#define VEC_CFG_DTOR          _trs_free
#define VEC_CFG_PREFIX        trs_
#define VEC_CFG_VEC           trs_vec
#define VEC_CFG_IMPLEMENTATION
#include <utils/vec.h>

static struct trs_vec trvec[1] = {0};

static void * _tradd_new_entry (size_t size, void * ptr, const char * file, const char * func, unsigned short line)
{
    if (ptr)
        trs_insert_sorted(trvec, _trs_new(size, ptr, file, func, line));
    return ptr;
}

void * _trcalloc (size_t nmemb, size_t size, const char * file, const char * func, unsigned short line)
{
    return _tradd_new_entry(size * nmemb, TRALLOC_CFG_CALLOC(nmemb, size), file, func, line);
}

void _trfree (void * ptr)
{
    if (!ptr) return;
    size_t idx = trs_search(trvec, _trs_new(0, ptr, NULL, NULL, 0));
    if (idx < trs_len(trvec)) /* ptr was found */
        _trs_free(trs_remove(trvec, idx));
}

void * _trmalloc (size_t size, const char * file, const char * func, unsigned short line)
{
    return _tradd_new_entry(size, TRALLOC_CFG_MALLOC(size), file, func, line);
}

void * _trrealloc (void * ptr, size_t size, const char * file, const char * func, unsigned short line)
{
    /*
     * calling realloc(ptr, size) where ptr == NULL is equivalent
     * to calling TRALLOC_CFG_MALLOC(size)
     */
    if (!ptr)
        return _trmalloc(size, file, func, line);

    /*
     * calling realloc(ptr, size) where ptr != NULL and size == 0 is
     * equivalent to calling free(ptr) and it may return either NULL
     * or a pointer passable to free()
     */
    if (size == 0)
        return _trfree(ptr), NULL;

    void * ret = TRALLOC_CFG_REALLOC(ptr, size);

    if (ret) { /* allocation was successful */
        /* ptr should already be in trvec */
        size_t idx = trs_search(trvec, _trs_new(0, ptr, NULL, NULL, 0));
        if (idx < trs_len(trvec)) { /* ptr is in trvec */
            if (ret == ptr) {
                trs_set_nth(trvec, idx, _trs_new(size, ret, file, func, line));
            } else {
                trs_remove(trvec, idx);
                _tradd_new_entry(size, ret, file, func, line);
            }
        } else { /* ptr not in trvec */
            _tradd_new_entry(size, ret, file, func, line);
        }
    }

    return ret;
}

size_t _trfprint (FILE * stream)
{
    size_t ret = 0;
    if (stream && !trs_is_empty(trvec)) {
        for (trs_iter(trvec); trs_itering(trvec); trs_iter_next(trvec)) {
            struct trs tmp = trs_get_nth(trvec, trs_iter_idx(trvec));
            ret += tmp.size;
            fprintf(stream, "%s:%s():%hu %p %zu\n", tmp.file, tmp.func, tmp.line, tmp.ptr, tmp.size);
        }
        fprintf(stream, "Total: %zu\n", ret);
    }
    return ret;
}

size_t _treprint (void)
{
    return _trfprint(stderr);
}

size_t _trprint (void)
{
    return _trfprint(stdout);
}

void _trdeinit (void)
{
    *trvec = trs_free(*trvec);
}

size_t _trused (void)
{
    size_t ret = 0;
    for (trs_iter(trvec); trs_itering(trvec); trs_iter_next(trvec)) {
        struct trs tmp = trs_get_nth(trvec, trs_iter_idx(trvec));
        ret += tmp.size;
    }
    return ret;
}

#undef TRALLOC_CFG_IMPLEMENTATION
#endif /* TRALLOC_CFG_IMPLEMENTATION */

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
