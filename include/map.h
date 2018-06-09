/* map - v2018.06.09-1
*/

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
# define MAP_CFG_CONCAT(A, B)    A ## B
# define MAP_CFG_MAKE_STR1(A, B) MAP_CFG_CONCAT(A, B)
# define MAP_CFG_MAKE_STR(A)     MAP_CFG_MAKE_STR1(MAP_CFG_PREFIX, A)

/*
 * Type of the keys for the map to hold
 */
# ifndef MAP_CFG_KEY_DATA_TYPE
#  error "Must define MAP_CFG_KEY_DATA_TYPE"
# endif /* MAP_CFG_KEY_DATA_TYPE */

/*
 * Type of the values for the map to hold
 */
# ifndef MAP_CFG_VALUE_DATA_TYPE
#  error "Must define MAP_CFG_VALUE_DATA_TYPE"
# endif /* MAP_CFG_VALUE_DATA_TYPE */

/*
 * If the map name wasn't overwritten and the prefix wasn't
 * defined, the map name defaults to `map`
 */
# ifndef MAP_CFG_MAP
#  define MAP_CFG_MAP map
# endif /* MAP_CFG_MAP */

/*
 * If no prefix was defined, default to `map_`
 */
# ifndef MAP_CFG_PREFIX
#  define MAP_CFG_PREFIX MAP_CFG_MAKE_STR1(MAP_CFG_MAP, _)
# endif /* MAP_CFG_PREFIX */

/**
 * @brief The map type
 */
struct MAP_CFG_MAP {
    /** The map, an array of arrays of entries */
    struct {

        /** An array of entries */
        struct {

            /** The hash of the key of this entry */
            unsigned int hash;

            /** The key of this entry */
            MAP_CFG_KEY_DATA_TYPE key;

            /** The value of this entry */
            MAP_CFG_VALUE_DATA_TYPE value;
        } * entries;

        /** Its length */
        unsigned int length;

        /*
         * since a call to realloc() (even if decreasing size)
         * may fail, the total capacity has to be kept
         * (maybe could be free slots?)
         */
        /** Its capacity */
        unsigned int capacity;
    } * map;

    /** Its length */
    unsigned int size;
};

/*==========================================================
 * Function names
 *=========================================================*/
#define MAP_ADD       MAP_CFG_MAKE_STR(add)
#define MAP_FREE      MAP_CFG_MAKE_STR(free)
#define MAP_NEW       MAP_CFG_MAKE_STR(new)
#define MAP_WITH_SIZE MAP_CFG_MAKE_STR(with_size)

/*==========================================================
 * Function prototypes
 *
 * RETURN TYPE     FUNCTION NAME PARAMETER LIST
 *==========================================================*/
bool               MAP_ADD       (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key, MAP_CFG_VALUE_DATA_TYPE value);
bool               MAP_NEW       (struct MAP_CFG_MAP * self);
bool               MAP_WITH_SIZE (struct MAP_CFG_MAP * self, unsigned int size);
struct MAP_CFG_MAP MAP_FREE      (struct MAP_CFG_MAP self);

#ifdef MAP_CFG_IMPLEMENTATION

#define _MAP_ENTRY_CMP       MAP_CFG_MAKE_STR(_entry_cmp)
#define _MAP_INCREASE_LENGTH MAP_CFG_MAKE_STR(_increase_length)
#define _MAP_INSERT_SORTED   MAP_CFG_MAKE_STR(_insert_sorted)
#define _MAP_SEARCH          MAP_CFG_MAKE_STR(_search)

/*
 * Hash function for the keys
 */
# ifndef MAP_CFG_HASH_FUNC
#  error "Must define MAP_CFG_HASH_FUNC"
# endif /* MAP_CFG_HASH_FUNC */

/*
 * How to compare keys (like `strcmp()`)
 */
# ifndef MAP_CFG_KEY_CMP
#  error "Must define MAP_CFG_KEY_CMP"
# endif /* MAP_CFG_KEY_CMP */

# ifdef MAP_CFG_STATIC
#  undef MAP_CFG_STATIC
#  define MAP_CFG_STATIC static
# endif /* MAP_CFG_STATIC */

/*
 * <stdlib.h>
 *  malloc()
 *  calloc()
 *  realloc()
 *  free()
 *
 * <string.h>
 *  memmove()
 *  memset()
 */
#include <stdlib.h>
#include <string.h>

# ifndef MAP_CFG_MALLOC
#  define MAP_CFG_MALLOC malloc
# endif /* MAP_CFG_MALLOC */

# ifndef MAP_CFG_CALLOC
#  define MAP_CFG_CALLOC calloc
# endif /* MAP_CFG_CALLOC */

# ifndef MAP_CFG_REALLOC
#  define MAP_CFG_REALLOC realloc
# endif /* MAP_CFG_REALLOC */

# ifndef MAP_CFG_FREE
#  define MAP_CFG_FREE free
# endif /* MAP_CFG_FREE */

# ifndef MAP_CFG_DEFAULT_SIZE
#  define MAP_CFG_DEFAULT_SIZE 101
# endif /* MAP_CFG_DEFAULT_SIZE */

# if MAP_CFG_DEFAULT_SIZE < 2
#  error "MAP_CFG_DEFAULT_SIZE must be bigger than 2"
# endif /* MAP_CFG_DEFAULT_SIZE < 3 */

static bool _MAP_INCREASE_LENGTH (struct MAP_CFG_MAP * self, unsigned int idx)
{
    if (self->map[idx].length < self->map[idx].capacity)
        return true;

    void * entries = self->map[idx].entries;
    unsigned int cap = self->map[idx].capacity + 1;
    entries = MAP_CFG_REALLOC(entries,
            sizeof(*self->map[idx].entries) * cap);
    bool ret = entries != NULL;

    if (ret) {
        self->map[idx].entries = entries;
        self->map[idx].capacity++;
    }

    return ret;
}

static inline int _MAP_ENTRY_CMP (unsigned int ha, MAP_CFG_KEY_DATA_TYPE ka, unsigned int hb, MAP_CFG_KEY_DATA_TYPE kb)
{
    return (ha < hb) ?
        -1:
        (ha > hb) ?
        1:
        MAP_CFG_KEY_CMP(ka, kb);
}

static bool _MAP_SEARCH (const struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key, unsigned int hash, unsigned int idx, unsigned int * _i)
{
    bool ret = false;
    unsigned int i = 0;
    unsigned int len = self->map[idx].length;

    if (len == 0)
        goto out;

    unsigned int left = 0;
    unsigned int right = len - 1;

    while (!ret && left <= right) {
        i = left + ((right - left) >> 1);

        int cmp = _MAP_ENTRY_CMP(hash, key,
                self->map[idx].entries[i].hash,
                self->map[idx].entries[i].key);

        if (cmp == 0)
            ret = true;
        else if (cmp < 0)
            right = i - 1;
        else
            left = i + 1;
    }

out:
    if (_i != NULL)
        *_i = i;

    return ret;
}

static bool _MAP_INSERT_SORTED (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key, MAP_CFG_VALUE_DATA_TYPE value, unsigned int hash, unsigned int idx)
{
    unsigned int i = 0;
    unsigned int len = self->map[idx].length;

    bool exists = _MAP_SEARCH(self, key, hash, idx, &i);

    if (!exists) {
        if (!_MAP_INCREASE_LENGTH(self, idx))
            return false;

        /* move entries to the right */
        if (i < len) {
            void * src = &self->map[idx].entries[i];
            void * dst = &self->map[idx].entries[i + 1];
            memmove(dst, src, sizeof(*self->map[idx].entries) * (len - i));
        }
    }

    self->map[idx].entries[i].hash = hash;
    self->map[idx].entries[i].key = key;
    self->map[idx].entries[i].value = value;
    self->map[idx].length++;

    return true;
}

/**
 * @brief Adds an entry to @a self with @a key and @a value.
 *        @a self must have been successfully initialized with
 *        MAP_NEW() or MAP_WITH_SIZE()
 *
 * @param self The map
 * @param key The key
 * @param value The value
 *
 * @returns `true` if it successfully added the entry to the map.
 *          This function fails (returns `false`) if @a self isn't
 *          valid, or it wasn't possible to get space for the new entry
 */
bool MAP_ADD (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key, MAP_CFG_VALUE_DATA_TYPE value)
{
    if (self == NULL || self->size == 0 || self->map == NULL)
        return false;

    unsigned int hash = MAP_CFG_HASH_FUNC(key);
    unsigned int idx = hash % self->size;

    return _MAP_INSERT_SORTED(self, key, value, hash, idx);
}

bool MAP_NEW (struct MAP_CFG_MAP * self)
{
    return MAP_WITH_SIZE(self, MAP_CFG_DEFAULT_SIZE);
}

bool MAP_WITH_SIZE (struct MAP_CFG_MAP * self, unsigned int size)
{
    self->map = MAP_CFG_CALLOC(size, sizeof(*self->map));

    bool ret = self->map != NULL;

    if (ret) {
        self->size = size;
        size *= sizeof(*self->map);
        memset(self->map, 0, size);
    }

    return ret;
}

struct MAP_CFG_MAP MAP_FREE (struct MAP_CFG_MAP self)
{
    if (self.map != NULL) {
        for (unsigned int i = 0; i < self.size; i++) {

# if defined(MAP_CFG_VALUE_DTOR) || defined(MAP_CFG_KEY_DTOR)
            for (unsigned int j = 0; j < self.map[i].length; j++) {
#  ifdef MAP_CFG_VALUE_DTOR
                MAP_CFG_VALUE_DTOR(self.map[i].entries[j].value);
#  endif /* MAP_CFG_VALUE_DTOR */

#  ifdef MAP_CFG_KEY_DTOR
                MAP_CFG_KEY_DTOR(self.map[i].entries[j].key);
#  endif /* MAP_CFG_KEY_DTOR */
            }
# endif /* MAP_CFG_VALUE_DTOR || MAP_CFG_KEY_DTOR */

            MAP_CFG_FREE(self.map[i].entries);
        }

        MAP_CFG_FREE(self.map);
    }

    memset(&self, 0, sizeof(struct MAP_CFG_MAP));

    return self;
}

/*==========================================================
 * Implementation clean up
 *=========================================================*/

/*
 * Functions
 */
#undef _MAP_ENTRY_CMP
#undef _MAP_INCREASE_LENGTH
#undef _MAP_INSERT_SORTED
#undef _MAP_SEARCH

/*
 * Other
 */
#undef MAP_CFG_CALLOC
#undef MAP_CFG_DEFAULT_SIZE
#undef MAP_CFG_FREE
#undef MAP_CFG_HASH_FUNC
#undef MAP_CFG_MALLOC
#undef MAP_CFG_REALLOC
#undef MAP_CFG_STATIC

#endif /* MAP_CFG_IMPLEMENTATION */

/*==========================================================
 * Header clean up
 *=========================================================*/

/*
 * Functions
 */
#undef MAP_ADD
#undef MAP_FREE
#undef MAP_NEW
#undef MAP_WITH_SIZE

/*
 * Other
 */
#undef MAP_CFG_CONCAT
#undef MAP_CFG_KEY_DATA_TYPE
#undef MAP_CFG_MAKE_STR
#undef MAP_CFG_MAKE_STR1
#undef MAP_CFG_MAP
#undef MAP_CFG_PREFIX
#undef MAP_CFG_VALUE_DATA_TYPE

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
