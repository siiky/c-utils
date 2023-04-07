/* map - v2023.04.07-0
 *
 * A Hash Map type inspired by
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *
 * The most up to date version of this file can be found at
 * `include/utils/map.h` on [siiky/c-utils](https://git.sr.ht/~siiky/c-utils)
 * More usage examples can be found at `examples/map` on the link above
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
            unsigned hash;

            /** The key of this entry */
            MAP_CFG_KEY_DATA_TYPE key;

            /** The value of this entry */
            MAP_CFG_VALUE_DATA_TYPE value;
        } * entries;

        /** Number of entries in this entry array */
        unsigned length;

        /*
         * since a call to realloc() (even if decreasing size)
         * may fail, the total capacity has to be kept
         * (maybe could be free slots?)
         */
        /** Maximum number of entries the array can hold */
        unsigned capacity;
    } * table;

    /** Table size (fixed on initialization) */
    unsigned size;

    /** Number of entries stored currently */
    unsigned cardinal;

    /** A little cache */
    struct {
        /** Whether the following info is still valid */
        bool valid;

        /** Hash of the last entry that has been updated/looked up */
        unsigned hash;

        /** Index of the entry in the entry array */
        unsigned idx;
    } lc;

    /** An iterator */
    struct {
        /** Whether it is iterating */
        bool ing;

        /** The table index */
        unsigned tblidx;

        /** The entry index */
        unsigned entidx;
    } iter;
};

/*==========================================================
 * Function names
 *=========================================================*/
#define MAP_ADD       MAP_CFG_MAKE_STR(add)
#define MAP_CARDINAL  MAP_CFG_MAKE_STR(cardinal)
#define MAP_CONTAINS  MAP_CFG_MAKE_STR(contains)
#define MAP_FREE      MAP_CFG_MAKE_STR(free)
#define MAP_GET       MAP_CFG_MAKE_STR(get)
#define MAP_IS_EMPTY  MAP_CFG_MAKE_STR(is_empty)
#define MAP_ITER      MAP_CFG_MAKE_STR(iter)
#define MAP_ITERING   MAP_CFG_MAKE_STR(itering)
#define MAP_ITER_END  MAP_CFG_MAKE_STR(iter_end)
#define MAP_ITER_KEY  MAP_CFG_MAKE_STR(iter_key)
#define MAP_ITER_NEXT MAP_CFG_MAKE_STR(iter_next)
#define MAP_ITER_VAL  MAP_CFG_MAKE_STR(iter_val)
#define MAP_INIT       MAP_CFG_MAKE_STR(init)
#define MAP_REMOVE    MAP_CFG_MAKE_STR(remove)
#define MAP_RESIZE    MAP_CFG_MAKE_STR(resize)
#define MAP_WITH_SIZE MAP_CFG_MAKE_STR(with_size)

/*==========================================================
 * Function prototypes
 *==========================================================*/
MAP_CFG_KEY_DATA_TYPE   MAP_ITER_KEY  (const struct MAP_CFG_MAP * self);
MAP_CFG_VALUE_DATA_TYPE MAP_GET       (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key);
MAP_CFG_VALUE_DATA_TYPE MAP_ITER_VAL  (const struct MAP_CFG_MAP * self);
bool                    MAP_ADD       (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key, const MAP_CFG_VALUE_DATA_TYPE value);
bool                    MAP_CONTAINS  (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key);
bool                    MAP_IS_EMPTY  (const struct MAP_CFG_MAP * self);
bool                    MAP_ITER      (struct MAP_CFG_MAP * self);
bool                    MAP_ITERING   (const struct MAP_CFG_MAP * self);
bool                    MAP_ITER_END  (struct MAP_CFG_MAP * self);
bool                    MAP_ITER_NEXT (struct MAP_CFG_MAP * self);
bool                    MAP_INIT      (struct MAP_CFG_MAP * self);
bool                    MAP_REMOVE    (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key, MAP_CFG_VALUE_DATA_TYPE * value);
bool                    MAP_RESIZE    (struct MAP_CFG_MAP * self, unsigned new_size);
bool                    MAP_WITH_SIZE (struct MAP_CFG_MAP * self, unsigned size);
struct MAP_CFG_MAP      MAP_FREE      (struct MAP_CFG_MAP self);
unsigned                MAP_CARDINAL  (const struct MAP_CFG_MAP * self);

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

# ifndef MAP_MOD
/**
 * @brief Calculates an index to an entry array
 * @returns An index in range [ 0, @a size [ from @a hash and @a size
 */
#  define MAP_MOD(hash, size) ((hash) % (size))
# endif /* MAP_MOD */

/**
 * @brief Tries to decrease the total capacity of an entry array to
 *        its length
 * @param self The map
 * @param tblidx The index of the entry array
 * @returns `true` if the array was already at the minimum capacity or
 *          it successfully decreased it, `false` otherwise
 */
static bool _MAP_DECREASE_CAPACITY (struct MAP_CFG_MAP * self, unsigned tblidx)
{
    if (self->table[tblidx].length == self->table[tblidx].capacity)
        return true;

    if (self->table[tblidx].length == 0) { /* avoid double free */
        MAP_CFG_FREE(self->table[tblidx].entries);
        self->table[tblidx].entries = NULL;
        self->table[tblidx].capacity = 0;
        return true;
    }

    unsigned cap = self->table[tblidx].length;
    void * entries = MAP_CFG_REALLOC(self->table[tblidx].entries,
            sizeof(*self->table[tblidx].entries) * cap);
    bool ret = entries != NULL;

    if (ret) {
        self->table[tblidx].entries = entries;
        self->table[tblidx].capacity = cap;
    }

    return ret;
}

/**
 * @brief Compares two entries based on hash and key
 * @param ha Hash of the first entry
 * @param ka Key of the first entry
 * @param hb Hash of the second entry
 * @param kb Key of the second entry
 * @returns <0 if (ha, ka) < (hb, kb), or
 *          >0 if (ha, ka) > (hb, kb), or
 *           0 if (ha, ka) == (hb, kb).
 *           The keys are compared with MAP_CFG_KEY_CMP()
 *
 * entryCmp (ha, ka) (hb, kb)
 *     | ha < hb   = LT
 *     | ha > hb   = GT
 *     | otherwise = keyCmp ka kb
 *
 * This function establishes a strict total order on entries iff
 * MAP_CFG_KEY_CMP() establishes a strict total order on keys
 */
static inline int _MAP_ENTRY_CMP (unsigned ha, const MAP_CFG_KEY_DATA_TYPE ka, unsigned hb, const MAP_CFG_KEY_DATA_TYPE kb)
{
    return (ha < hb) ?
        -1:
        (ha > hb) ?
        1:
        MAP_CFG_KEY_CMP(ka, kb);
}

/**
 * @brief Tries to increase the total capacity of an entry array to
 *        fit another entry
 * @param self The map
 * @param tblidx The index of the entry array
 * @returns `true` if it successfully increased the capacity or wasn't
 *          necessary, `false` otherwise
 */
static bool _MAP_INCREASE_CAPACITY (struct MAP_CFG_MAP * self, unsigned tblidx)
{
    if (self->table[tblidx].length < self->table[tblidx].capacity)
        return true;

    unsigned cap = self->table[tblidx].capacity + 1;
    void * entries = MAP_CFG_REALLOC(self->table[tblidx].entries,
            sizeof(*self->table[tblidx].entries) * cap);
    bool ret = entries != NULL;

    if (ret) {
        self->table[tblidx].entries = entries;
        self->table[tblidx].capacity++;
    }

    return ret;
}

/**
 * @brief Searches for an entry with key @a key and hash @a hash in
 *        the entry array with index @a tblidx
 * @param self The map
 * @param key The key
 * @param hash The hash of @a key
 * @param tblidx The index of the entry array
 * @param[out] _i The index of the entry in the entry array (!NULL)
 * @returns `true` if there was an entry with key @a key, and sets
 *          @a _i to the index of the entry in the entry array.
 *          `false` if there was no entry with key @a key, and sets
 *          @a _i to the index in the entry array where an entry
 *          with key @a key should be inserted
 *
 * This function assumes the entry arrays are sorted according to
 *     _MAP_ENTRY_CMP(). This means MAP_CFG_KEY_CMP() must be defined similarly
 *     strcmp(), the only difference being the arguments are not pointers to,
 *     but values of type MAP_CFG_KEY_DATA_TYPE. It may be defined either as a
 *     macro or a function, but must behave as if it was a function of the
 *     following type:
 *
 * int MAP_CFG_KEY_CMP (MAP_CFG_KEY_DATA_TYPE, MAP_CFG_KEY_DATA_TYPE)
 */
static bool _MAP_SEARCH (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key, unsigned hash, unsigned tblidx, unsigned * _i)
{
    bool ret = false;
    unsigned i = 0;
    unsigned size = self->table[tblidx].length;
    unsigned base = 0;

    if (size == 0)
        goto out;

    if (self->lc.valid
            && self->lc.hash == hash
            && MAP_CFG_KEY_CMP(key, self->table[tblidx].entries[self->lc.idx].key) == 0)
    {
        i = self->lc.idx;
        ret = true;
        goto out;
    }

    while (size > 1) {
        unsigned half = size >> 1;
        unsigned mid = base + half;

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

    i = base + (!ret && cmp < 0);

    if (ret) {
        self->lc.valid = true;
        self->lc.hash = hash;
        self->lc.idx = i;
    }

out:
    *_i = i;
    return ret;
}

/**
 * @brief Inserts or updates an entry
 * @param self The map
 * @param key The key
 * @param value The value
 * @param hash The hash of @a key
 * @param tblidx The index of the entry array where the entry should
 *        be put
 * @returns `false` if there was no entry with key @a key and it wasn't
 *          possible to insert it, `true` otherwise
 */
static bool _MAP_INSERT_SORTED (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key, const MAP_CFG_VALUE_DATA_TYPE value, unsigned hash, unsigned tblidx)
{
    unsigned i = 0;
    bool exists = _MAP_SEARCH(self, key, hash, tblidx, &i);

    if (!exists) {
        if (!_MAP_INCREASE_CAPACITY(self, tblidx))
            return false;

        /* move entries to the right */
        unsigned len = self->table[tblidx].length;
        if (i < len)
            memmove(&self->table[tblidx].entries[i + 1],
                    &self->table[tblidx].entries[i],
                    sizeof(*self->table[tblidx].entries) * (len - i));

        self->table[tblidx].entries[i].hash = hash;
        self->table[tblidx].entries[i].key = key;
        self->table[tblidx].length++;
        self->cardinal++;
    }

    self->table[tblidx].entries[i].value = value;
    self->lc.valid = true;
    self->lc.hash = hash;
    self->lc.idx = i;

    return true;
}

/**
 * @brief Gets the key of the iterator's current entry.
 *        The map must be iterating
 * @param self The map
 * @returns The key of the iterator's current entry
 */
MAP_CFG_KEY_DATA_TYPE MAP_ITER_KEY (const struct MAP_CFG_MAP * self)
{
    assert(self->iter.ing);
    assert(self->iter.tblidx < self->size);
    assert(self->iter.entidx < self->table[self->iter.tblidx].length);
    return self->table[self->iter.tblidx].entries[self->iter.entidx].key;
}

/**
 * @brief Gets the value associated with a given key The map must have been
 *        successfully initialized with MAP_INIT() or MAP_WITH_SIZE()
 * @param self The map
 * @param key The key (must be the key of an entry in the map)
 * @returns The value associated with @a key
 */
MAP_CFG_STATIC MAP_CFG_VALUE_DATA_TYPE MAP_GET (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key)
{
    assert(self != NULL);
    assert(self->size >= 3);
    assert(self->table != NULL);

    unsigned hash = MAP_CFG_HASH_FUNC(key);
    unsigned tblidx = MAP_MOD(hash, self->size);
    unsigned i = 0;

    bool exists = _MAP_SEARCH(self, key, hash, tblidx, &i);
    assert(exists);
    return self->table[tblidx].entries[i].value;
}

/**
 * @brief Gets the value of the iterator's current entry.
 *        The map must be iterating
 * @param self The map
 * @returns The value of the iterator's current entry
 */
MAP_CFG_VALUE_DATA_TYPE MAP_ITER_VAL (const struct MAP_CFG_MAP * self)
{
    assert(self->iter.ing);
    assert(self->iter.tblidx < self->size);
    assert(self->iter.entidx < self->table[self->iter.tblidx].length);
    return self->table[self->iter.tblidx].entries[self->iter.entidx].value;
}

/**
 * @brief Adds or updates an entry to the map with @a key and @a value. The map
 *        must have been successfully initialized with MAP_INIT() or
 *        MAP_WITH_SIZE()
 * @param self The map
 * @param key The key
 * @param value The value
 * @returns `true` if it successfully added the entry to the map.
 *          This function fails (returns `false`) if the map isn't
 *          valid, or it wasn't possible to get space for the new entry
 */
MAP_CFG_STATIC bool MAP_ADD (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key, const MAP_CFG_VALUE_DATA_TYPE value)
{
    if (self == NULL || self->size < 3 || self->table == NULL)
        return false;

    unsigned hash = MAP_CFG_HASH_FUNC(key);
    unsigned tblidx = MAP_MOD(hash, self->size);

    return _MAP_INSERT_SORTED(self, key, value, hash, tblidx);
}

/**
 * @brief Checks if the map contains a given @a key
 * @param self The map
 * @param key The key
 * @returns `true` if the map contains @a key
 */
MAP_CFG_STATIC bool MAP_CONTAINS (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key)
{
    if (self == NULL || self->size < 3 || self->table == NULL)
        return false;

    unsigned hash = MAP_CFG_HASH_FUNC(key);
    unsigned tblidx = MAP_MOD(hash, self->size);

    unsigned _i;
    return _MAP_SEARCH(self, key, hash, tblidx, &_i);
}

/**
 * @brief Checks if the map is empty (i.e., has no entries)
 * @param self The map
 * @returns `true` if the map is empty, `false` otherwise
 */
bool MAP_IS_EMPTY (const struct MAP_CFG_MAP * self)
{
    return MAP_CARDINAL(self) == 0;
}

/**
 * @brief Starts iterating over the map
 * @param self The map
 * @returns `false` if the map is already iterating or is empty
 */
bool MAP_ITER (struct MAP_CFG_MAP * self)
{
    if (self->iter.ing || MAP_IS_EMPTY(self))
        return false;

    unsigned tblidx = 0;
    for (; tblidx < self->size && self->table[tblidx].length == 0; tblidx++)
        ;

    self->iter.ing = true;
    self->iter.tblidx = tblidx;
    self->iter.entidx = 0;

    return true;
}

/**
 * @brief Checks if the map is iterating
 * @param self The map
 * @returns `true` if the map is iterating
 */
bool MAP_ITERING (const struct MAP_CFG_MAP * self)
{
    return self && self->iter.ing;
}

/**
 * @brief Stops iterating over the map. Has no effect if is wasn't
 *        iterating
 * @param self The map
 * @returns `true` if the map is not NULL
 */
bool MAP_ITER_END (struct MAP_CFG_MAP * self)
{
    return self
        && !(self->iter.ing = false);
}

/**
 * @brief Advances the iterator to the next entry (if any)
 * @param self The map
 * @returns `true` if the map is still iterating
 */
bool MAP_ITER_NEXT (struct MAP_CFG_MAP * self)
{
    if (!MAP_ITERING(self)
            || self->iter.tblidx >= self->size
            || self->iter.entidx >= self->table[self->iter.tblidx].length)
        return false;

    if (self->iter.entidx < self->table[self->iter.tblidx].length - 1)
        return self->iter.entidx++, true;

    unsigned tblidx = 0;
    for (; tblidx < self->size && self->table[tblidx].length == 0; tblidx++)
        ;

    bool ret = tblidx < self->size;
    if (ret) {
        self->iter.tblidx = tblidx;
        self->iter.entidx = 0;
    }

    return ret;
}

/**
 * @brief Initializes a map with the default size
 * @param self The map
 * @returns `true` if it successfully initialized the map
 */
MAP_CFG_STATIC bool MAP_INIT (struct MAP_CFG_MAP * self)
{
    return MAP_WITH_SIZE(self, MAP_CFG_DEFAULT_SIZE);
}

/**
 * @brief Remove the entry with a given key
 * @param self The map
 * @param key The key
 * @param[out] value Where to save the value associated with @a key. If it is
 *             NULL the value is free()d.
 * @retuns `true` if there was an entry with key @a key, or `false` if there
 *         was no such entry or the map is not valid
 *
 * If defined, MAP_CFG_KEY_DTOR() and MAP_CFG_VALUE_DTOR() are called on the
 *     entry to be removed
 */
MAP_CFG_STATIC bool MAP_REMOVE (struct MAP_CFG_MAP * self, const MAP_CFG_KEY_DATA_TYPE key, MAP_CFG_VALUE_DATA_TYPE * value)
{
    if (self == NULL || self->size < 3 || self->table == NULL)
        return false;

    unsigned hash = MAP_CFG_HASH_FUNC(key);
    unsigned tblidx = MAP_MOD(hash, self->size);

    unsigned i = 0;
    bool exists = _MAP_SEARCH(self, key, hash, tblidx, &i);
    if (!exists)
        return false;

#ifdef MAP_CFG_KEY_DTOR
    MAP_CFG_KEY_DTOR(self->table[tblidx].entries[i].key);
#endif /* MAP_CFG_KEY_DTOR */

    if (value != NULL)
        *value = self->table[tblidx].entries[i].value;
#ifdef MAP_CFG_VALUE_DTOR
    else
        MAP_CFG_VALUE_DTOR(self->table[tblidx].entries[i].value);
#endif /* MAP_CFG_VALUE_DTOR */

    self->table[tblidx].length--;
    memmove(&self->table[tblidx].entries[i],
            &self->table[tblidx].entries[i + 1],
            sizeof(*self->table[tblidx].entries) * (self->table[tblidx].length - i));

    _MAP_DECREASE_CAPACITY(self, tblidx);

    self->lc.valid = false;
    self->cardinal--;

    return true;
}

/**
 * @brief Resizes a map
 * @param self The map
 * @param new_size The new size for the map
 * @returns `true` if it successfully resized the map,
 *          `false` otherwise
 *
 * Initializes a map with the requested size and tries to insert every
 *     entry of the original map into the new one. After all entries
 *     have been inserted, the original map is freed. This is not very
 *     efficient memory-wise, but reduces the risk of corrupting the
 *     original map. If any of the insertions fail, the new map is
 *     freed and the original is left untouched.
 */
MAP_CFG_STATIC bool MAP_RESIZE (struct MAP_CFG_MAP * self, unsigned new_size)
{
    if (self == NULL) return false;

    unsigned cur_size = self->size;
    if (cur_size == new_size) return true;

    struct MAP_CFG_MAP ret = {0};
    if (!MAP_WITH_SIZE(&ret, new_size))
        return false;

    for (unsigned tblidx = 0; tblidx < cur_size; tblidx++) {
        unsigned length = self->table[tblidx].length;

        for (unsigned entidx = 0; entidx < length; entidx++) {
            MAP_CFG_KEY_DATA_TYPE key = self->table[tblidx].entries[entidx].key;
            MAP_CFG_VALUE_DATA_TYPE val = self->table[tblidx].entries[entidx].value;
            unsigned hash = self->table[tblidx].entries[entidx].hash;
            unsigned targtblidx = MAP_MOD(hash, new_size);

            if (!_MAP_INSERT_SORTED(&ret, key, val, hash, targtblidx))
                goto ret_cleanup;
        }
    }

    /* self cleanup */
    for (unsigned tblidx = 0; tblidx < cur_size; tblidx++)
        if (self->table[tblidx].entries != NULL)
            MAP_CFG_FREE(self->table[tblidx].entries);
    MAP_CFG_FREE(self->table);

    return (*self = ret), true;

ret_cleanup:
    for (unsigned tblidx = 0; tblidx < new_size; tblidx++)
        if (ret.table[tblidx].entries != NULL)
            MAP_CFG_FREE(ret.table[tblidx].entries);
    MAP_CFG_FREE(ret.table);
    return false;
}

/**
 * @brief Initializes a map with a given size
 * @param self The map
 * @param size The size of the table (must be >= 3)
 * @returns `true` if it successfully initialized the map
 */
MAP_CFG_STATIC bool MAP_WITH_SIZE (struct MAP_CFG_MAP * self, unsigned size)
{
    if (self == NULL || size < 3)
        return false;

    *self = (struct MAP_CFG_MAP) {0};
    self->table = MAP_CFG_CALLOC(size, sizeof(*self->table));

    bool ret = self->table != NULL;

    if (ret)
        self->size = size;

    return ret;
}

/**
 * @brief Cleans and frees the map. It also frees keys and values if
 *        MAP_CFG_KEY_DTOR() and MAP_CFG_VALUE_DTOR() are defined
 * @param self The map
 * @returns A new empty (clean) map
 */
MAP_CFG_STATIC struct MAP_CFG_MAP MAP_FREE (struct MAP_CFG_MAP self)
{
    if (self.table != NULL) {
        for (unsigned i = 0; i < self.size; i++) {
            if (self.table[i].entries != NULL) {

# if defined(MAP_CFG_VALUE_DTOR) || defined(MAP_CFG_KEY_DTOR)
                for (unsigned j = 0; j < self.table[i].length; j++) {
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

/**
 * @brief Calculates the cardinal (number of entries) in the map
 * @param self The map
 * @returns The number of entries in the map
 */
unsigned MAP_CARDINAL (const struct MAP_CFG_MAP * self)
{
    return (self) ? self->cardinal : 0;
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
#undef MAP_CARDINAL
#undef MAP_CONTAINS
#undef MAP_FREE
#undef MAP_GET
#undef MAP_IS_EMPTY
#undef MAP_ITER
#undef MAP_ITERING
#undef MAP_ITER_END
#undef MAP_ITER_KEY
#undef MAP_ITER_NEXT
#undef MAP_ITER_VAL
#undef MAP_INIT
#undef MAP_REMOVE
#undef MAP_RESIZE
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
