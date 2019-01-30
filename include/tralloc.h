#ifndef _TRALLOC_H
#define _TRALLOC_H

/*
 * <stdbool.h>
 *  bool
 *  false
 *  true
 *
 * <stdio.h>
 *  FILE
 *  fprintf()
 *  stderr
 *  stdout
 */
#include <stdbool.h>
#include <stdio.h>

/*
 * Define TRALLOC_CFG_TRACE to use tralloc
 */
#ifndef TRALLOC_CFG_TRACE

#    define trdeinit() ((void)0)

#    define trcalloc  calloc
#    define trfree    free
#    define trmalloc  malloc
#    define trrealloc realloc

/* if trace is not enabled, the tr*print() macros work as if there were no leaks */
#    define treprint()     true
#    define trfprint(OUTF) true
#    define trprint()      true

#else

#    define trdeinit()            _trdeinit()

#    define trcalloc(nmemb, size) _trcalloc((nmemb), (size), __FILE__, __func__, __LINE__)
#    define trfree(ptr)           _trfree((ptr))
#    define trmalloc(size)        _trmalloc((size), __FILE__, __func__, __LINE__)
#    define trrealloc(ptr, size)  _trrealloc((ptr), (size), __FILE__, __func__, __LINE__)

#    define treprint()            _treprint()
#    define trfprint(OUTF)        _trfprint((OUTF))
#    define trprint()             _trprint()

#endif /* TRALLOC_CFG_TRACE */

/**
 * @brief If there is unfreed memory, print (to stderr) each block
 *        in the format "file:function:line address size"
 * @returns `true` if there's no unfreed memory, `false` otherwise
 */
bool _treprint (void);

/**
 * @brief If there is unfreed memory, and @a stream is not NULL,
 *        print (to @a stream) each block in the format
 *        "file:function:line address size".
 * @param stream Where to write
 * @returns `true` if there's no unfreed memory, `false` otherwise
 */
bool _trfprint (FILE * stream);

/**
 * @brief If there is unfreed memory, print (to stdout) each block
 *        in the format "file:function:line address size"
 * @returns `true` if there's no unfreed memory, `false` otherwise
 */
bool _trprint (void);

/**
 * @brief Remove @a ptr from the list of unfreed memory and free it
 * @param ptr A pointer to dynamically allocated memory
 *
 * If @a ptr is not in the list, it works similarly to `stdlib.h`'s `free()`
 */
void _trfree (void * ptr);

/**
 * @brief Wrapper for `stdlib.h`'s `calloc()`. Record where the allocation
 *        occurred (@a file, @a func, @a line), its size (@a nmemb * @a size)
 *        and the address returned by `calloc()`
 * @param nmemb Number of members
 * @param size Size of each member
 * @param file filename
 * @param func function name
 * @param line Line number
 * @returns Same as `calloc()`
 */
void * _trcalloc (size_t nmemb, size_t size, const char * file, const char * func, unsigned short line);

/**
 * @brief Wrapper for `stdlib.h`'s `malloc()`. Record where the allocation
 *        occurred (@a file, @a func, @a line), its size (@a size)
 *        and the address returned by `malloc()`
 * @param size Size of each member
 * @param file filename
 * @param func function name
 * @param line Line number
 * @returns Same as `malloc()`
 */
void * _trmalloc (size_t size, const char * file, const char * func, unsigned short line);

/**
 * @brief Wrapper for `stdlib.h`'s `realloc()`. Record where the allocation
 *        occurred (@a file, @a func, @a line), its size (@a size)
 *        and the new address returned by `realloc()`. If @a ptr is
 *        already in the list, it is updated (if needed). If @a ptr
 *        is NULL, _trmalloc() is called instead. If @a size is 0,
 *        _trfree() is called instead, and NULL is returned.
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

#endif /* _TRALLOC_H */

#ifdef TRALLOC_CFG_IMPLEMENTATION
/*
 * <stdlib.h>
 *  calloc()
 *  free()
 *  malloc()
 *  realloc()
 */
#include <stdlib.h>

struct trs {
    /* file where this block of memory was allocated */
    const char * file;
    /* function where this block of memory was allocated */
    const char * func;
    /* line where this block of memory was allocated */
    unsigned short line;

    /* size of this block of memory */
    size_t size;
    /* pointer to the allocated block of memory */
    void * ptr;
};

static int trs_cmp (struct trs a, struct trs b)
{
    return a.ptr != b.ptr;
}

static struct trs _trs_free (struct trs self)
{
    if (self.ptr)
        free(self.ptr);
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
        trs_push(trvec, _trs_new(size, ptr, file, func, line));
    return ptr;
}

void * _trcalloc (size_t nmemb, size_t size, const char * file, const char * func, unsigned short line)
{
    return _tradd_new_entry(size * nmemb, calloc(nmemb, size), file, func, line);
}

void _trfree (void * ptr)
{
    if (ptr) {
        size_t idx = trs_find(trvec, _trs_new(0, ptr, NULL, NULL, 0));

        if (idx < trs_len(trvec)) /* ptr was found */
            _trs_free(trs_swap_remove(trvec, idx));
    }
}

void * _trmalloc (size_t size, const char * file, const char * func, unsigned short line)
{
    return _tradd_new_entry(size, malloc(size), file, func, line);
}

void * _trrealloc (void * ptr, size_t size, const char * file, const char * func, unsigned short line)
{
    /*
     * calling realloc(ptr, size) where ptr == NULL is equivalent
     * to calling malloc(size)
     */
    if (!ptr)
        return _trmalloc(size, file, func, line);

    /*
     * calling realloc(ptr, size) where ptr != NULL and size == 0 is
     * equivalent to calling free(ptr) and it may return either NULL
     * or a pointer passable to free()
     */
    if (size == 0) {
        _trfree(ptr);
        return NULL;
    }

    void * ret = NULL;
    size_t idx = 0;
    bool elem = false;

    /* ptr should already be in trvec */
    {
        idx = trs_find(trvec, _trs_new(0, ptr, NULL, NULL, 0));
        elem = idx < trs_len(trvec);
    }

    ret = realloc(ptr, size);

    if (ret) { /* allocation was successful */
        if (elem) /* ptr is in trvec */
            trs_set_nth(trvec, idx, _trs_new(size, ret, file, func, line));
        else /* ptr not in trvec */
            _tradd_new_entry(size, ret, file, func, line);
    }

    return ret;
}

bool _trfprint (FILE * stream)
{
    bool ret = trs_is_empty(trvec);
    if (stream && !ret) {
        size_t total = 0;

        for (trs_iter(trvec); trs_itering(trvec); trs_iter_next(trvec)) {
            struct trs tmp = trs_get_nth(trvec, trs_iter_idx(trvec));
            total += tmp.size;
            fprintf(stream, "%s:%s():%hu %p %zu\n", tmp.file, tmp.func, tmp.line, tmp.ptr, tmp.size);
        }

        fprintf(stream, "Total: %zu\n", total);
    }
    return ret;
}

bool _treprint (void)
{
    return _trfprint(stderr);
}

bool _trprint (void)
{
    return _trfprint(stdout);
}

void _trdeinit (void)
{
    *trvec = trs_free(*trvec);
}

#undef TRALLOC_CFG_IMPLEMENTATION
#endif /* TRALLOC_CFG_IMPLEMENTATION */
