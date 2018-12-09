/* map - v2018.12.09-0
 *
 * A Hash Map type inspired by
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *
 * The most up to date version of this file can be found at
 * `include/map.h` on [siiky/c-utils](https://github.com/siiky/c-utils)
 * More usage examples can be found at `src/map` on the link above
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
    } * table;

    /** Its length */
    unsigned int size;

    /** Whether the following info is still valid */
    bool lc_is_valid;

    /** Hash of the last entry that has been updated/looked up */
    unsigned int lc_hash;

    /** Index of the entry in the entry array */
    unsigned int lc_idx;
};

/*==========================================================
 * Function names
 *=========================================================*/
#define MAP_ADD       MAP_CFG_MAKE_STR(add)
#define MAP_CONTAINS  MAP_CFG_MAKE_STR(contains)
#define MAP_FREE      MAP_CFG_MAKE_STR(free)
#define MAP_GET       MAP_CFG_MAKE_STR(get)
#define MAP_NEW       MAP_CFG_MAKE_STR(new)
#define MAP_REMOVE    MAP_CFG_MAKE_STR(remove)
#define MAP_WITH_SIZE MAP_CFG_MAKE_STR(with_size)

/*==========================================================
 * Function prototypes
 *
 * RETURN TYPE            FUNCTION NAME PARAMETER LIST
 *==========================================================*/
MAP_CFG_VALUE_DATA_TYPE MAP_GET       (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key);
bool                    MAP_ADD       (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key, MAP_CFG_VALUE_DATA_TYPE value);
bool                    MAP_CONTAINS  (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key);
bool                    MAP_NEW       (struct MAP_CFG_MAP * self);
bool                    MAP_REMOVE    (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key);
bool                    MAP_WITH_SIZE (struct MAP_CFG_MAP * self, unsigned int size);
struct MAP_CFG_MAP      MAP_FREE      (struct MAP_CFG_MAP self);

#ifdef MAP_CFG_IMPLEMENTATION

#define _MAP_DECREASE_CAPACITY MAP_CFG_MAKE_STR(_decrease_capacity)
#define _MAP_ENTRY_CMP         MAP_CFG_MAKE_STR(_entry_cmp)
#define _MAP_INCREASE_CAPACITY MAP_CFG_MAKE_STR(_increase_capacity)
#define _MAP_INSERT_SORTED     MAP_CFG_MAKE_STR(_insert_sorted)
#define _MAP_SEARCH            MAP_CFG_MAKE_STR(_search)

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
# else /* MAP_CFG_STATIC */
#  undef MAP_CFG_STATIC
#  define MAP_CFG_STATIC
# endif /* MAP_CFG_STATIC */

/*
 * <assert.h>
 *  assert()
 *
 * <stdlib.h>
 *  calloc()
 *  free()
 *  malloc()
 *  realloc()
 *
 * <string.h>
 *  memmove()
 *  memset()
 */
#include <assert.h>
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

# if MAP_CFG_DEFAULT_SIZE < 3
#  error "MAP_CFG_DEFAULT_SIZE must be bigger than 2"
# endif /* MAP_CFG_DEFAULT_SIZE < 3 */

static bool _MAP_DECREASE_CAPACITY (struct MAP_CFG_MAP * self, unsigned int tblidx)
{
    if (self->table[tblidx].length == self->table[tblidx].capacity)
        return true;

    unsigned int cap = self->table[tblidx].length;
    void * entries = MAP_CFG_REALLOC(self->table[tblidx].entries,
            sizeof(*self->table[tblidx].entries) * cap);
    bool ret = entries != NULL;

    if (ret) {
        self->table[tblidx].entries = entries;
        self->table[tblidx].capacity = cap;
    }

    return ret;
}

static bool _MAP_INCREASE_CAPACITY (struct MAP_CFG_MAP * self, unsigned int tblidx)
{
    if (self->table[tblidx].length < self->table[tblidx].capacity)
        return true;

    unsigned int cap = self->table[tblidx].capacity + 1;
    void * entries = MAP_CFG_REALLOC(self->table[tblidx].entries,
            sizeof(*self->table[tblidx].entries) * cap);
    bool ret = entries != NULL;

    if (ret) {
        self->table[tblidx].entries = entries;
        self->table[tblidx].capacity++;
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

static bool _MAP_SEARCH (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key, unsigned int hash, unsigned int tblidx, unsigned int * _i)
{
    bool ret = false;
    unsigned int i = 0;
    unsigned int size = self->table[tblidx].length;
    unsigned int base = 0;

    if (size == 0)
        goto out;

    if (self->lc_is_valid
            && self->lc_hash == hash
            && MAP_CFG_KEY_CMP(key, self->table[tblidx].entries[self->lc_idx].key) == 0)
    {
        i = self->lc_idx;
        ret = true;
        goto out;
    }

    while (size > 1) {
        unsigned int half = size >> 1;
        unsigned int mid = base + half;

        int cmp = _MAP_ENTRY_CMP(hash, key,
                self->table[tblidx].entries[mid].hash,
                self->table[tblidx].entries[mid].key);

        base = (cmp > 0) ?
            base :
            mid;

        size -= half;
    }

    int cmp = _MAP_ENTRY_CMP(hash, key,
            self->table[tblidx].entries[base].hash,
            self->table[tblidx].entries[base].key);

    ret = cmp == 0;

    i = (ret) ?
        base :
        base + (cmp < 0);

    if (ret) {
        self->lc_is_valid = true;
        self->lc_hash = hash;
        self->lc_idx = i;
    }

out:
    *_i = i;
    return ret;
}

static bool _MAP_INSERT_SORTED (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key, MAP_CFG_VALUE_DATA_TYPE value, unsigned int hash, unsigned int tblidx)
{
    unsigned int i = 0;
    unsigned int len = self->table[tblidx].length;

    bool exists = _MAP_SEARCH(self, key, hash, tblidx, &i);

    if (!exists) {
        if (!_MAP_INCREASE_CAPACITY(self, tblidx))
            return false;

        /* move entries to the right */
        if (i < len)
            memmove(&self->table[tblidx].entries[i + 1],
                    &self->table[tblidx].entries[i],
                    sizeof(*self->table[tblidx].entries) * (len - i));
    }

    self->table[tblidx].entries[i].hash = hash;
    self->table[tblidx].entries[i].key = key;
    self->table[tblidx].entries[i].value = value;
    self->table[tblidx].length++;

    self->lc_is_valid = true;
    self->lc_hash = hash;
    self->lc_idx = i;

    return true;
}

MAP_CFG_STATIC MAP_CFG_VALUE_DATA_TYPE MAP_GET (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key)
{
    assert(self != NULL);
    assert(self->size >= 3);
    assert(self->table != NULL);

    unsigned int hash = MAP_CFG_HASH_FUNC(key);
    unsigned int tblidx = hash % self->size;
    unsigned int i = 0;

    bool exists = _MAP_SEARCH(self, key, hash, tblidx, &i);

    assert(exists);
    return self->table[tblidx].entries[i].value;
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
MAP_CFG_STATIC bool MAP_ADD (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key, MAP_CFG_VALUE_DATA_TYPE value)
{
    if (self == NULL || self->size < 3 || self->table == NULL)
        return false;

    unsigned int hash = MAP_CFG_HASH_FUNC(key);
    unsigned int tblidx = hash % self->size;

    return _MAP_INSERT_SORTED(self, key, value, hash, tblidx);
}

MAP_CFG_STATIC bool MAP_CONTAINS (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key)
{
    if (self == NULL || self->size < 3 || self->table == NULL)
        return false;

    unsigned int hash = MAP_CFG_HASH_FUNC(key);
    unsigned int tblidx = hash % self->size;

    unsigned int _i;
    return _MAP_SEARCH(self, key, hash, tblidx, &_i);
}

MAP_CFG_STATIC bool MAP_NEW (struct MAP_CFG_MAP * self)
{
    return MAP_WITH_SIZE(self, MAP_CFG_DEFAULT_SIZE);
}

MAP_CFG_STATIC bool MAP_REMOVE (struct MAP_CFG_MAP * self, MAP_CFG_KEY_DATA_TYPE key)
{
    if (self == NULL || self->size < 3 || self->table == NULL)
        return false;

    unsigned int hash = MAP_CFG_HASH_FUNC(key);
    unsigned int tblidx = hash % self->size;

    unsigned int i = 0;
    bool exists = _MAP_SEARCH(self, key, hash, tblidx, &i);
    if (!exists)
        return false;

#ifdef MAP_CFG_KEY_DTOR
    MAP_CFG_KEY_DTOR(self->table[tblidx].entries[i].key);
#endif /* MAP_CFG_KEY_DTOR */

#ifdef MAP_CFG_VALUE_DTOR
    MAP_CFG_VALUE_DTOR(self->table[tblidx].entries[i].value);
#endif /* MAP_CFG_VALUE_DTOR */

    memmove(&self->table[tblidx].entries[i],
            &self->table[tblidx].entries[i + 1],
            sizeof(*self->table[tblidx].entries) * (self->table[tblidx].length - i - 1));

    _MAP_DECREASE_CAPACITY(self, tblidx);

    self->lc_is_valid = false;

    return true;
}

MAP_CFG_STATIC bool MAP_WITH_SIZE (struct MAP_CFG_MAP * self, unsigned int size)
{
    if (self == NULL || self->size < 3)
        return false;

    self->table = MAP_CFG_CALLOC(size, sizeof(*self->table));

    bool ret = self->table != NULL;

    if (ret) {
        self->lc_is_valid = false;
        self->size = size;
        memset(self->table, 0, size * sizeof(*self->table));
    }

    return ret;
}

MAP_CFG_STATIC struct MAP_CFG_MAP MAP_FREE (struct MAP_CFG_MAP self)
{
    if (self.table != NULL) {
        for (unsigned int i = 0; i < self.size; i++) {
            if (self.table[i].entries != NULL) {

# if defined(MAP_CFG_VALUE_DTOR) || defined(MAP_CFG_KEY_DTOR)
                for (unsigned int j = 0; j < self.table[i].length; j++) {
#  ifdef MAP_CFG_VALUE_DTOR
                    MAP_CFG_VALUE_DTOR(self.table[i].entries[j].value);
#  endif /* MAP_CFG_VALUE_DTOR */

#  ifdef MAP_CFG_KEY_DTOR
                    MAP_CFG_KEY_DTOR(self.table[i].entries[j].key);
#  endif /* MAP_CFG_KEY_DTOR */
                }
# endif /* MAP_CFG_VALUE_DTOR || MAP_CFG_KEY_DTOR */

                MAP_CFG_FREE(self.table[i].entries);
            }
        }

        MAP_CFG_FREE(self.table);
    }

    return (struct MAP_CFG_MAP) {0};
}

/*==========================================================
 * Implementation clean up
 *=========================================================*/

/*
 * Functions
 */
#undef _MAP_DECREASE_CAPACITY
#undef _MAP_ENTRY_CMP
#undef _MAP_INCREASE_CAPACITY
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
#undef MAP_CONTAINS
#undef MAP_FREE
#undef MAP_GET
#undef MAP_NEW
#undef MAP_REMOVE
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
