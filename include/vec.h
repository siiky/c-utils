/*
 * A vector type inspired by
 *  * Rust's `Vec` type
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *
 * # Usage
 *
 * ```c
 * #include <assert.h>
 *
 * #define VEC_DATA_TYPE size_t
 *
 * // Optionally, define an equality function
 * // (Not optional for compound types, like structs)
 * #define VEC_DATA_TYPE_EQ(L, R) ((L) == (R))
 *
 * // Optionally, define a prefix (defaults to `vec_`)
 * #define VEC_PREFIX my_
 *
 * // Optionally, define NDEBUG to disable asserts inside vec.h
 * #define NDEBUG
 *
 * // Optionally, define VEC_STATIC to mark definitions as static
 * // Probably will raise `unused-function` warnings
 * #define VEC_STATIC
 *
 * #include <vec.h>
 *
 * int main (void)
 * {
 *     struct my_vec vec = my_new();
 *
 *     size_t used = 0;
 *     for (size_t i = 0; i < 100; i++)
 *         if (my_push(&vec, i)) used++;
 *
 *     size_t len = my_len(&vec);
 *     assert(used == len);
 *     assert(len >= my_capacity(&vec));
 *     for (my_iter(&vec); my_itering(&vec); my_iter_next(&vec)) {
 *         size_t r = my_get_nth(&vec, my_iter_idx(&vec));
 *         assert(r == i);
 *     }
 *
 *     // You can give a destructor function to `my_free()`
 *     // to apply to every element of the vector
 *     // If NULL, only `free()` is called
 *     my_free(&vec, NULL);
 *
 *     assert(my_as_slice(&vec) == NULL);
 *     assert(my_len(&vec) == 0);
 *     assert(my_capacity(&vec) == 0);
 *
 *     return 0;
 * }
 * ```
 *
 * # TODO
 *
 * - [X] Allow more than one implementation for different types
 * - [ ] More options
 */

/*
 * <assert.h>
 *  assert()
 *
 * <stdbool.h>
 *  bool
 *  false
 *  true
 *
 * <stddef.h>
 *  size_t
 *
 * <stdlib.h>
 *  bsearch()
 *  calloc()
 *  qsort()
 *  realloc()
 *
 * <string.h>
 *  memcpy()
 *  memset()
 */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
 * Type of data for the vector to hold
 */
#ifndef VEC_DATA_TYPE
# error "Must declare VEC_DATA_TYPE"
#endif /* VEC_DATA_TYPE */

/*
 * If no prefix was defined, default to `vec_`
 */
#ifndef VEC_PREFIX
# define VEC_PREFIX vec_
#endif /* VEC_PREFIX */

/*
 * Magic from `sort.h`
 */
#define VEC_CONCAT(A, B)    A ## B
#define VEC_MAKE_STR1(A, B) VEC_CONCAT(A, B)
#define VEC_MAKE_STR(A)     VEC_MAKE_STR1(VEC_PREFIX, A)

#define VEC_VEC VEC_MAKE_STR(vec)
/**=========================================================
 * @brief The vector type
 */
struct VEC_VEC {
    /** Pointer to actual data */
    VEC_DATA_TYPE * ptr;
    /** Number of elements */
    size_t length;
    /** Number of elements the vector can hold currently */
    size_t capacity;
    /** Iterator index */
    size_t idx;
    /** Should iterate from begginning or end */
    unsigned char reverse : 1;
    /** Is currently iterating */
    unsigned char iterating : 1;
};

# ifndef VEC_DATA_TYPE_EQ
#  define VEC_DATA_TYPE_EQ(L, R) ((L) == (R))
# endif /* VEC_DATA_TYPE_EQ */

# ifdef VEC_STATIC
#  undef VEC_STATIC
#  define VEC_STATIC static
# else /* VEC_STATIC */
#  undef VEC_STATIC
#  define VEC_STATIC
# endif /* VEC_STATIC */

/*==========================================================
 * Function names
 *=========================================================*/
#define VEC_APPEND             VEC_MAKE_STR(append)
#define VEC_AS_MUT_SLICE       VEC_MAKE_STR(as_mut_slice)
#define VEC_AS_SLICE           VEC_MAKE_STR(as_slice)
#define VEC_BSEARCH            VEC_MAKE_STR(bsearch)
#define VEC_CAPACITY           VEC_MAKE_STR(capacity)
#define VEC_ELEM               VEC_MAKE_STR(elem)
#define VEC_FILTER             VEC_MAKE_STR(filter)
#define VEC_FIND               VEC_MAKE_STR(find)
#define VEC_FREE               VEC_MAKE_STR(free)
#define VEC_FROM_RAW_PARTS     VEC_MAKE_STR(from_raw_parts)
#define VEC_GET_NTH            VEC_MAKE_STR(get_nth)
#define VEC_INSERT             VEC_MAKE_STR(insert)
#define VEC_IS_EMPTY           VEC_MAKE_STR(is_empty)
#define VEC_ITER               VEC_MAKE_STR(iter)
#define VEC_ITERING            VEC_MAKE_STR(itering)
#define VEC_ITER_END           VEC_MAKE_STR(iter_end)
#define VEC_ITER_IDX           VEC_MAKE_STR(iter_idx)
#define VEC_ITER_NEXT          VEC_MAKE_STR(iter_next)
#define VEC_ITER_REV           VEC_MAKE_STR(iter_rev)
#define VEC_LEN                VEC_MAKE_STR(len)
#define VEC_MAP                VEC_MAKE_STR(map)
#define VEC_NEW                VEC_MAKE_STR(new)
#define VEC_POP                VEC_MAKE_STR(pop)
#define VEC_PUSH               VEC_MAKE_STR(push)
#define VEC_QSORT              VEC_MAKE_STR(qsort)
#define VEC_REMOVE             VEC_MAKE_STR(remove)
#define VEC_RESERVE            VEC_MAKE_STR(reserve)
#define VEC_SET_LEN            VEC_MAKE_STR(set_len)
#define VEC_SET_NTH            VEC_MAKE_STR(set_nth)
#define VEC_SHRINK_TO_FIT      VEC_MAKE_STR(shrink_to_fit)
#define VEC_SPLIT_OFF          VEC_MAKE_STR(split_off)
#define VEC_SWAP_REMOVE        VEC_MAKE_STR(swap_remove)
#define VEC_TRUNCATE           VEC_MAKE_STR(truncate)
#define VEC_WITH_CAPACITY      VEC_MAKE_STR(with_capacity)
#define _VEC_DECREASE_CAPACITY VEC_MAKE_STR(_decrease_capacity)
#define _VEC_INCREASE_CAPACITY VEC_MAKE_STR(_increase_capacity)

/*==========================================================
 * Function prototypes
 *
 * RETURN TYPE        FUNCTION NAME      PARAMETER LIST
 *==========================================================*/
VEC_DATA_TYPE         VEC_GET_NTH        (const struct VEC_VEC * self, size_t nth);
VEC_DATA_TYPE         VEC_POP            (struct VEC_VEC * self);
VEC_DATA_TYPE         VEC_REMOVE         (struct VEC_VEC * self, size_t index);
VEC_DATA_TYPE         VEC_SWAP_REMOVE    (struct VEC_VEC * self, size_t index);
VEC_DATA_TYPE *       VEC_AS_MUT_SLICE   (struct VEC_VEC * self);
bool                  VEC_APPEND         (struct VEC_VEC * restrict self, struct VEC_VEC * restrict other);
bool                  VEC_ELEM           (const struct VEC_VEC * self, VEC_DATA_TYPE element);
bool                  VEC_FILTER         (struct VEC_VEC * self, bool pred (VEC_DATA_TYPE *));
bool                  VEC_FREE           (struct VEC_VEC * self, VEC_DATA_TYPE dtor (VEC_DATA_TYPE));
bool                  VEC_INSERT         (struct VEC_VEC * self, size_t index, VEC_DATA_TYPE element);
bool                  VEC_IS_EMPTY       (const struct VEC_VEC * self);
bool                  VEC_ITER           (struct VEC_VEC * self);
bool                  VEC_ITERING        (const struct VEC_VEC * self);
bool                  VEC_ITER_END       (struct VEC_VEC * self);
bool                  VEC_ITER_NEXT      (struct VEC_VEC * self);
bool                  VEC_ITER_REV       (struct VEC_VEC * self, bool rev);
bool                  VEC_MAP            (struct VEC_VEC * self, VEC_DATA_TYPE f (VEC_DATA_TYPE));
bool                  VEC_PUSH           (struct VEC_VEC * self, VEC_DATA_TYPE element);
bool                  VEC_QSORT          (struct VEC_VEC * self, int compar (const void *, const void *));
bool                  VEC_RESERVE        (struct VEC_VEC * self, size_t total);
bool                  VEC_SET_LEN        (struct VEC_VEC * self, size_t len);
bool                  VEC_SET_NTH        (struct VEC_VEC * self, size_t nth, VEC_DATA_TYPE element);
bool                  VEC_SHRINK_TO_FIT  (struct VEC_VEC * self);
bool                  VEC_TRUNCATE       (struct VEC_VEC * self, size_t len);
const VEC_DATA_TYPE * VEC_AS_SLICE       (const struct VEC_VEC * self);
size_t                VEC_BSEARCH        (const struct VEC_VEC * self, VEC_DATA_TYPE key, int compar (const void *, const void *));
size_t                VEC_CAPACITY       (const struct VEC_VEC * self);
size_t                VEC_FIND           (const struct VEC_VEC * self, VEC_DATA_TYPE element);
size_t                VEC_ITER_IDX       (struct VEC_VEC * self);
size_t                VEC_LEN            (const struct VEC_VEC * self);
struct VEC_VEC        VEC_FROM_RAW_PARTS (VEC_DATA_TYPE * ptr, size_t length, size_t capacity);
struct VEC_VEC        VEC_NEW            (void);
struct VEC_VEC        VEC_SPLIT_OFF      (struct VEC_VEC * self, size_t at);
struct VEC_VEC        VEC_WITH_CAPACITY  (size_t capacity);

#ifdef VEC_IMPLEMENTATION

/*==========================================================
 * Function definitions
 *========================================================*/

/**=========================================================
 * @brief Check if @a self has capacity for another element, and try
 *        to increase it
 * @param self The vector
 * @returns `true` if @a self has enough space for at least one more
 *          element, `false` otherwise
 */
static inline bool _VEC_INCREASE_CAPACITY (struct VEC_VEC * self)
{
    if (self == NULL)
        return false;

    if (self->length >= self->capacity)
        return true;

    /* new_cap = (cap * 1.5) + 1 */
    size_t new_cap = self->capacity + (self->capacity >> 1) + 1;

    return VEC_RESERVE(self, new_cap);
}


/**=========================================================
 * @brief Check if @a self has too much unused memory and decrease it
 * @param self The vector
 * @returns `false` if @a self has too much unused memory but it
 *          couldn't decrease it, `true` otherwise
 */
static inline bool _VEC_DECREASE_CAPACITY (struct VEC_VEC * self)
{
    /* more than half of the capacity in use? */
    if (self->length > (self->capacity >> 1))
        return true;

    /* new_cap = len * 1.5 */
    /* len * 1.5 ~ cap * 0.75 */
    size_t new_cap = self->length + (self->length >> 1);
    VEC_DATA_TYPE * new = realloc(self->ptr, new_cap * sizeof(VEC_DATA_TYPE));
    if (new != NULL) {
        self->ptr = new;
        self->capacity = new_cap;
    }

    return new != NULL;
}
/**=========================================================
 * @brief Free @a self, applying @a dtor on every element
 * @param self The vector
 * @param dtor A function to apply on every element of @a self
 * @returns `false` if @a self is `NULL`, `true` otherwise
 */
VEC_STATIC bool VEC_FREE (struct VEC_VEC * self, VEC_DATA_TYPE dtor (VEC_DATA_TYPE))
{
    if (self == NULL)
        return false;

    if (dtor != NULL)
        VEC_MAP(self, dtor);

    if (self->ptr != NULL)
        free(self->ptr);

    memset(self, 0, sizeof(struct VEC_VEC));

    return true;
}

/**=========================================================
 * @brief Create a new empty vector
 * @returns The new vector
 */
VEC_STATIC inline struct VEC_VEC VEC_NEW (void)
{
    return VEC_FROM_RAW_PARTS(NULL, 0, 0);
}

/**=========================================================
 * @brief Create a new vector with @a capacity free slots
 * @param capacity Number of elements to allocate
 * @returns The new vector
 */
VEC_STATIC inline struct VEC_VEC VEC_WITH_CAPACITY (size_t capacity)
{
    VEC_DATA_TYPE * data = calloc(capacity, sizeof(VEC_DATA_TYPE));
    capacity = (data != NULL) ? capacity : 0;
    return VEC_FROM_RAW_PARTS(data, 0, capacity);
}

/**=========================================================
 * @brief Create a vector from separate components
 * @param ptr A pointer to allocated memory
 * @param length Number of elements in @a ptr
 * @param capacity Total number of elements @a ptr can hold
 * @returns A new vector pointing to @a ptr, with @a length and @a capacity
 */
VEC_STATIC inline struct VEC_VEC VEC_FROM_RAW_PARTS (VEC_DATA_TYPE * ptr, size_t length, size_t capacity)
{
    return (struct VEC_VEC) {
        .ptr = ptr,
        .length = length,
        .capacity = capacity,
        .idx = 0,
        .reverse = 0,
        .iterating = 0,
    };
}

/**=========================================================
 * @brief Calculate the capacity of @a self
 * @param self The vector
 * @returns The capacity of @a self
 */
VEC_STATIC inline size_t VEC_CAPACITY (const struct VEC_VEC * self)
{
    return (self != NULL) ?
        self->capacity :
        0;
}

/**=========================================================
 * @brief Reserve memory for @a total elements
 * @param self The vector
 * @param total Number of total elements
 * @returns `true` if @a self already had enough capacity or it was
 *          able to reserve the extra capacity needed, `false` otherwise
 */
VEC_STATIC bool VEC_RESERVE (struct VEC_VEC * self, size_t total)
{
    if (self == NULL)
        return false;

    if (self->capacity >= total)
        return true;

    VEC_DATA_TYPE * ptr = realloc(self->ptr, total * sizeof(VEC_DATA_TYPE));
    if (ptr != NULL) {
        self->ptr = ptr;
        self->capacity = total;
    }

    return ptr != NULL;
}

/**=========================================================
 * @brief Shrink the capacity of @a self to its length
 * @param self The vector
 * @returns `false` if it wasn't possible to shrink @a self,
 *          `true` otherwise
 */
VEC_STATIC bool VEC_SHRINK_TO_FIT (struct VEC_VEC * self)
{
    if (self == NULL)
        return false;

    VEC_DATA_TYPE * ptr = realloc(self->ptr, self->length * sizeof(VEC_DATA_TYPE));
    if (ptr != NULL) {
        self->ptr = ptr;
        self->capacity = self->length;
    }

    return ptr != NULL;
}

/**=========================================================
 * @brief Shorten @a self, keeping the first @a len elements
 * @param self The vector
 * @param len New length
 * @returns `false` if @a self is not a valid vector, `true` otherwise
 */
VEC_STATIC bool VEC_TRUNCATE (struct VEC_VEC * self, size_t len)
{
    if (self == NULL)
        return false;

    if (self->length > len)
        self->length = len;

    return true;
}

/**=========================================================
 * @brief Return a `const` qualified pointer to allocated memory
 * @param self The vector
 * @returns `const` qualified pointer to allocated memory
 */
VEC_STATIC inline const VEC_DATA_TYPE * VEC_AS_SLICE (const struct VEC_VEC * self)
{
    return (self != NULL) ?
        (const VEC_DATA_TYPE *) self->ptr :
        NULL;
}

/**=========================================================
 * @brief Like vec_as_slice, but not `const` qualified
 * @param self The vector
 * @returns Pointer to allocated memory
 */
VEC_STATIC inline VEC_DATA_TYPE * VEC_AS_MUT_SLICE (struct VEC_VEC * self)
{
    return (self != NULL) ?
        self->ptr :
        NULL;
}

/**=========================================================
 * @brief Set the length of @a self to @a len, without checks
 * @param self The vector
 * @param len The new length
 * @returns `false` if @a self not a valid vector, `true` otherwise
 */
VEC_STATIC inline bool VEC_SET_LEN (struct VEC_VEC * self, size_t len)
{
    if (self == NULL)
        return false;
    self->length = len;
    return true;
}

/**=========================================================
 * @brief Remove the element at @a index, replacing it with the last
 *        element
 * @param self The vector
 * @param index Index of the element to be removed
 * @returns The removed element
 */
VEC_STATIC VEC_DATA_TYPE VEC_SWAP_REMOVE (struct VEC_VEC * self, size_t index)
{
    assert(self != NULL);
    assert(self->ptr != NULL);
    assert(self->length > 0);
    assert(index < self->length);

    VEC_DATA_TYPE ret = self->ptr[index];
    self->ptr[index] = VEC_POP(self);

    return ret;
}

/**=========================================================
 * @brief Insert an @a element at @a index, shifting every element
 *        before it to the right
 * @param self The vector
 * @param index Where the element will be inserted
 * @param element Element to be inserted
 * @returns `false` if @a index is out of bounds or @a self didn't have
 *          enough capacity and it wasn't possible to increase it,
 *          `true` otherwise
 */
VEC_STATIC bool VEC_INSERT (struct VEC_VEC * self, size_t index, VEC_DATA_TYPE element)
{
    if (self == NULL
    /* allow inserting at the head */
    || (VEC_IS_EMPTY(self) && index > 0)
    || index >= self->length
    || !_VEC_INCREASE_CAPACITY(self))
        return false;

    for (size_t i = self->length; i >= index; i--)
        self->ptr[i] = self->ptr[i - 1];

    self->length++;
    self->ptr[index] = element;

    return true;
}

/**=========================================================
 * @brief Remove an element at @a index, shifting every element after
 *        it to the left
 * @param self The vector
 * @param index Index of the element to remove
 * @returns The removed element
 */
VEC_STATIC VEC_DATA_TYPE VEC_REMOVE (struct VEC_VEC * self, size_t index)
{
    assert(self != NULL);
    assert(self->ptr != NULL);
    assert(index < self->length);

    VEC_DATA_TYPE ret = self->ptr[index];

    for (size_t i = index + 1; i < self->length; i++)
        self->ptr[i - 1] = self->ptr[i];

    self->length--;

    _VEC_DECREASE_CAPACITY(self);

    return ret;
}

/**=========================================================
 * @brief Keep every element of @a self that satisfies a predicate
 *        @a pred
 * @param self The vector
 * @param pred The predicate
 * @returns `false` if @a self is not a valid vector or @a pred is
 *          NULL, `true` otherwise
 */
VEC_STATIC bool VEC_FILTER (struct VEC_VEC * self, bool pred (VEC_DATA_TYPE *))
{
    if (self == NULL
    || self->ptr == NULL
    || pred == NULL)
        return false;

    size_t len = 0;

    for (size_t r = 0; r < self->length; r++)
        if (pred(self->ptr + r))
            self->ptr[len++] = self->ptr[r];

    self->length = len;

    _VEC_DECREASE_CAPACITY(self);

    return true;
}

/**=========================================================
 * @brief Insert an @a element at the end of @a self
 * @param self The vector
 * @param element Element to be pushed
 * @returns `false` if @a self is not a valid vector, or it didn't
 *          have enough capacity and it wasn't possible to increase
 *          it, `true` otherwise
 */
VEC_STATIC bool VEC_PUSH (struct VEC_VEC * self, VEC_DATA_TYPE element)
{
    if (self == NULL || !_VEC_INCREASE_CAPACITY(self))
        return false;

    self->ptr[self->length] = element;
    self->length++;

    return true;
}

/**=========================================================
 * @brief Remove the last element of @a self
 * @param self The vector
 * @returns The removed element
 */
VEC_STATIC VEC_DATA_TYPE VEC_POP (struct VEC_VEC * self)
{
    assert(self != NULL);
    assert(self->ptr != NULL);
    assert(self->length > 0);

    self->length--;

    _VEC_DECREASE_CAPACITY(self);

    return self->ptr[self->length];
}

/**=========================================================
 * @brief Append @a other to @a self and set the length of @a other
 *        to 0
 * @param self The vector
 * @param other The other vector
 * @returns `false` if either @a self or @a other aren't valid vectors,
 *          @a self didn't have enough capacity and it wasn't possible
 *          to increase it, `true` otherwise
 */
VEC_STATIC bool VEC_APPEND (struct VEC_VEC * restrict self, struct VEC_VEC * restrict other)
{
    if (self == NULL
    || other == NULL
    || self->ptr == NULL
    || other->ptr == NULL
    || self->ptr == other->ptr
    || !VEC_RESERVE(self, self->length + other->length))
        return false;

    VEC_DATA_TYPE * dest = self->ptr + self->length;
    VEC_DATA_TYPE * src = other->ptr;
    size_t n = other->length * sizeof(VEC_DATA_TYPE);
    memcpy(dest, src, n);

    other->length = 0;

    return true;
}

/**=========================================================
 * @brief Calculate the length of @a self
 * @param self The vector
 * @returns The length of @a self
 */
VEC_STATIC inline size_t VEC_LEN (const struct VEC_VEC * self)
{
    return (self != NULL) ?
        self->length :
        0;
}

/**=========================================================
 * @brief Check if @a self is empty
 * @param self The vector
 * @returns `true` if @a self is empty, `false` otherwise
 */
VEC_STATIC inline bool VEC_IS_EMPTY (const struct VEC_VEC * self)
{
    return self == NULL
        || self->ptr == NULL
        || self->length == 0;
}

/**=========================================================
 * @brief Split @a self in two, keeping the first `at - 1` elements
 *        in @a self, and the rest in the returned vector
 * @param self The vector
 * @param at Where to split
 * @returns The new vector
 */
VEC_STATIC struct VEC_VEC VEC_SPLIT_OFF (struct VEC_VEC * self, size_t at)
{
    assert(self != NULL);
    assert(self->ptr != NULL);
    assert(at < self->length);
    assert(at > 0);

    struct VEC_VEC ret = VEC_WITH_CAPACITY(self->length - at + 1);

    if (ret.ptr != NULL) {
        void * dest = ret.ptr;
        const void * src = self->ptr + at - 1;
        size_t n = sizeof(VEC_DATA_TYPE) * ret.capacity;
        memcpy(dest, src, n);

        ret.length = ret.capacity;
        self->length -= ret.capacity;
    }

    return ret;
}

/**=========================================================
 * @brief Get the element at the @a nth index
 * @param self The vector
 * @param nth The index
 * @returns The element at index @a nth
 */
VEC_STATIC inline VEC_DATA_TYPE VEC_GET_NTH (const struct VEC_VEC * self, size_t nth)
{
    assert(self != NULL);
    assert(self->ptr != NULL);
    assert(nth < self->length);
    return self->ptr[nth];
}

/**=========================================================
 * @brief Set the element at index @a nth to @a element
 * @param self The vector
 * @param nth The index
 * @param element The new element
 * @returns `false` if @a self is not a valid vector or @a nth is out
 *          of bounds, `true` otherwise
 */
VEC_STATIC inline bool VEC_SET_NTH (struct VEC_VEC * self, size_t nth, VEC_DATA_TYPE element)
{
    if (self == NULL
    || self->ptr == NULL
    || nth >= self->length)
        return false;
    self->ptr[nth] = element;
    return true;
}

/**=========================================================
 * @brief Get the index of the first occurrence (if any) of @a element
 * @param self The vector
 * @param element The element to look for
 * @returns The index of the first occurrence of @a element, or, if
 *          @a element does not exist, the length of @a self
 */
VEC_STATIC size_t VEC_FIND (const struct VEC_VEC * self, VEC_DATA_TYPE element)
{
    assert(self != NULL);
    assert(self->ptr != NULL);

    size_t ret = 0;
    for (ret = 0;
            ret < self->length
            && !(VEC_DATA_TYPE_EQ(self->ptr[ret], element));
            ret++);

    return ret;
}

/**=========================================================
 * @brief Check if @a self contains an @a element
 * @param self The vector
 * @param element The element to look for
 * @returns `true` if @a element exists in @a self, `false` otherwise
 */
VEC_STATIC inline bool VEC_ELEM (const struct VEC_VEC * self, VEC_DATA_TYPE element)
{
    return self != NULL
        && self->ptr != NULL
        && (VEC_FIND(self, element) < self->length);
}

/**=========================================================
 * @brief Wraper for `stdlib.h`'s `bsearch()` function
 * @param self The vector
 * @param key The key to look for
 * @param compar A function suitable to be passed to `bsearch()`
 * @returns The index of an occurrence of @a element, or, if @a element
 *          does not exist, the length of @a self
 */
VEC_STATIC size_t VEC_BSEARCH (const struct VEC_VEC * self, VEC_DATA_TYPE key, int compar (const void *, const void *))
{
    assert(self != NULL);
    assert(self->ptr != NULL);
    assert(compar != NULL);

    const void * base = self->ptr;
    size_t nmemb = self->length;
    size_t size = sizeof(VEC_DATA_TYPE);

    VEC_DATA_TYPE * found = bsearch(&key, base, nmemb, size, compar);
    return (found != NULL) ?
        (size_t) (found - self->ptr) :
        self->length;
}

/**=========================================================
 * @brief Wraper for `stdlib.h`'s `qsort()` function
 * @param self The vector
 * @param compar A function suitable to be passed to `qsort()`
 * @returns `false` if @a self is not a valid vector, `true` otherwise
 */
VEC_STATIC bool VEC_QSORT (struct VEC_VEC * self, int compar (const void *, const void *))
{
    if (self == NULL || self->ptr == NULL || compar == NULL)
        return false;
    qsort(self->ptr, self->length, sizeof(VEC_DATA_TYPE), compar);
    return true;
}

/**=========================================================
 * @brief Apply @a f to every element of @a self
 * @param self The vector
 * @param f The function to apply on every element
 * @returns `false` if @a self is not a valid vector, `true` otherwise
 */
VEC_STATIC bool VEC_MAP (struct VEC_VEC * self, VEC_DATA_TYPE f (VEC_DATA_TYPE))
{
    if (self == NULL || self->ptr == NULL || f == NULL)
        return false;
    size_t len = VEC_LEN(self);
    for (size_t i = 0; i < len; i++)
        self->ptr[i] = f(self->ptr[i]);
    return true;
}

/**=========================================================
 * @brief Start iterating over @a self. If @a self is already iterating,
 *        do nothing
 * @param self The vector
 * @returns `false` if @a self is not a valid vector or it is already
 *          iterating, `true` otherwise
 */
VEC_STATIC bool VEC_ITER (struct VEC_VEC * self)
{
    if (self == NULL
    || self->iterating
    || self->ptr == NULL
    || self->length == 0)
        return false;

    self->idx = (self->reverse) ?
        self->length - 1 :
        0;

    self->iterating = true;

    return true;
}

/**=========================================================
 * @brief Stop the iterator over @a self
 * @param self The vector
 * @returns `false` if @a self is not a valid vector, `true` otherwise
 */
VEC_STATIC inline bool VEC_ITER_END (struct VEC_VEC * self)
{
    return (self != NULL) && !(self->iterating = false);
}

/**=========================================================
 * @brief Check if @a self is iterating
 * @param self The vector
 * @returns `true` if @a self is iterating, `false` otherwise
 */
VEC_STATIC inline bool VEC_ITERING (const struct VEC_VEC * self)
{
    return (self != NULL) && self->iterating;
}

/**=========================================================
 * @brief Get the index of the iterator
 * @param self The vector
 * @returns The current index of the iterator
 */
VEC_STATIC inline size_t VEC_ITER_IDX (struct VEC_VEC * self)
{
    assert(self != NULL);
    assert(self->idx < self->length);
    return self->idx;
}

/**=========================================================
 * @brief Move the iterator to the next index
 * @param self The vector
 * @returns `true` if still iterating, `false` otherwise
 */
VEC_STATIC bool VEC_ITER_NEXT (struct VEC_VEC * self)
{
    if (self == NULL || self->idx >= self->length || !self->iterating)
        return false;

    bool over = self->idx == ((!self->reverse) ?
            self->length - 1 :
            0);

    if (over)
        VEC_ITER_END(self);
    else
        if (self->reverse)
            self->idx--;
        else
            self->idx++;

    return self->iterating;
}

/**=========================================================
 * @brief Set the `reverse` flag, i.e., if the iterator should
 *        move from beggining to end or end to begginning
 * @param self The vector
 * @returns `false` if @a self is iterating or it is not a valid vector,
 *          `true` otherwise
 */
VEC_STATIC bool VEC_ITER_REV (struct VEC_VEC * self, bool rev)
{
    if (self == NULL || self->iterating)
        return false;
    self->reverse = rev;
    return true;
}

#endif /* VEC_IMPLEMENTATION */

/*==========================================================
 * Clean up
 *=========================================================*/

/*
 * Functions
 */
#undef VEC_APPEND
#undef VEC_AS_MUT_SLICE
#undef VEC_AS_SLICE
#undef VEC_BSEARCH
#undef VEC_CAPACITY
#undef VEC_ELEM
#undef VEC_FILTER
#undef VEC_FIND
#undef VEC_FREE
#undef VEC_FROM_RAW_PARTS
#undef VEC_GET_NTH
#undef VEC_INSERT
#undef VEC_IS_EMPTY
#undef VEC_ITER
#undef VEC_ITERING
#undef VEC_ITER_END
#undef VEC_ITER_IDX
#undef VEC_ITER_NEXT
#undef VEC_ITER_REV
#undef VEC_LEN
#undef VEC_MAP
#undef VEC_NEW
#undef VEC_POP
#undef VEC_PUSH
#undef VEC_QSORT
#undef VEC_REMOVE
#undef VEC_RESERVE
#undef VEC_SET_LEN
#undef VEC_SET_NTH
#undef VEC_SHRINK_TO_FIT
#undef VEC_SPLIT_OFF
#undef VEC_SWAP_REMOVE
#undef VEC_TRUNCATE
#undef VEC_WITH_CAPACITY
#undef _VEC_DECREASE_CAPACITY
#undef _VEC_INCREASE_CAPACITY

/*
 * Other
 */
#undef VEC_DATA_TYPE
#undef VEC_DATA_TYPE_EQ
#undef VEC_PREFIX
#undef VEC_STATIC
#undef VEC_VEC

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
