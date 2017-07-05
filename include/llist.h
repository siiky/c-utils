/*
 * A Double-Linked Circular List inspired by
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *  
 *  # Usage
 *
 *  ```c
 *  #include <assert.h>
 *
 *  #define LLIST_DATA_TYPE size_t
 *
 * // Optionally, define an equality function
 * // (Not optional for compound types, like structs)
 * #define LLIST_DATA_TYPE_EQ(L, R) ((L) == (R))
 * 
 * // Optionally, define a prefix (defaults to `llist_`)
 * #define LLIST_PREFIX your_prefix_
 *
 * // Optionally, define LLIST_STATIC to mark definitions as static
 * // Probably will raise `unused-function` warnings
 * #define LLIST_STATIC
 *
 * #include <llist.h>
 *
 * int main (void)
 * {
 *     struct your_prefix_LList list = your_prefix_new();
 *
 *     for (size_t i = 0; i < 10; i++)
 *         your_prefix_append(&list, i);
 *
 *     for (size_t i = 0; i < 10; i++)
 *         assert(your_prefix_nth(&list, i) == i);
 *
 *     your_prefix_free(&list);
 *
 *     assert(list.length == NULL);
 *     assert(list.head == NULL);
 *     assert(list.last == NULL);
 *
 *     return 0;
 * }
 *
 *  ```
 */

#include <assert.h>  /* assert() */
#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* calloc(),
                        realloc() */
#include <string.h>  /* memcpy() */

/*
 * Type of data for the List to hold
 */
#ifndef LLIST_DATA_TYPE
# error "Must declare LLIST_DATA_TYPE"
#endif /* LLIST_DATA_TYPE */

/*
 * If no prefix was defined, default to `llist_`
 */
#ifndef LLIST_PREFIX
# define LLIST_PREFIX llist_
#endif /* LLIST_PREFIX */

/*
 * Magic from `sort.h`
 */
#define LLIST_CONCAT(A, B)    A ## B
#define LLIST_MAKE_STR1(A, B) LLIST_CONCAT(A, B)
#define LLIST_MAKE_STR(A)     LLIST_MAKE_STR1(LLIST_PREFIX, A)

#define LLIST_NODE LLIST_MAKE_STR(Node)
/**=========================================================
 */
struct LLIST_NODE {
    /** Value of this node. */
    LLIST_DATA_TYPE val;
    /** Previous node in the List. */
    struct LLIST_NODE * prev;
    /** Next node in the List. */
    struct LLIST_NODE * next;
};

#define LLIST_LLIST LLIST_MAKE_STR(LList)
/**=========================================================
 */
struct LLIST_LLIST {
    /** Length of the List. */
    size_t length;
    /** Head of the List. */
    struct LLIST_NODE * head;
    /** Last node of the List. */
    struct LLIST_NODE * last;
};

# ifndef LLIST_DATA_TYPE_EQ
#  define LLIST_DATA_TYPE_EQ(L, R) ((L) == (R))
# endif /* LLIST_DATA_TYPE_EQ */

# ifdef LLIST_STATIC
#  undef LLIST_STATIC
#  define LLIST_STATIC static
# else /* LLIST_STATIC */
#  undef LLIST_STATIC
#  define LLIST_STATIC
# endif /* LLIST_STATIC */

/*==========================================================
 * Function names
==========================================================*/
#define LLIST_NEW               LLIST_MAKE_STR(new)
#define LLIST_FREE              LLIST_MAKE_STR(free)
#define LLIST_LEN               LLIST_MAKE_STR(len)
#define LLIST_NTH               LLIST_MAKE_STR(nth)
#define LLIST_NODE_NEW          LLIST_MAKE_STR(node_new)
#define LLIST_EMPTY_ADD         LLIST_MAKE_STR(empty_add)
#define LLIST_NON_EMPTY_ADD     LLIST_MAKE_STR(non_empty_add)
#define LLIST_CONS              LLIST_MAKE_STR(cons)
#define LLIST_APPEND            LLIST_MAKE_STR(append)
#define LLIST_MAP               LLIST_MAKE_STR(map)

/*==========================================================
 * Function prototypes
==========================================================*/
LLIST_STATIC struct LLIST_LLIST LLIST_NEW (void);
LLIST_STATIC void LLIST_FREE (struct LLIST_LLIST * self);
LLIST_STATIC size_t LLIST_LEN (const struct LLIST_LLIST * self);
LLIST_STATIC LLIST_DATA_TYPE LLIST_NTH (const struct LLIST_LLIST * self, size_t nth);
LLIST_STATIC struct LLIST_NODE * LLIST_NODE_NEW (LLIST_DATA_TYPE val, struct LLIST_NODE * prev, struct LLIST_NODE * next);
LLIST_STATIC void LLIST_EMPTY_ADD (struct LLIST_LLIST * self, LLIST_DATA_TYPE val);
LLIST_STATIC struct LLIST_NODE * LLIST_NON_EMPTY_ADD (struct LLIST_LLIST * self, LLIST_DATA_TYPE val);
LLIST_STATIC void LLIST_CONS (struct LLIST_LLIST * self, LLIST_DATA_TYPE val);
LLIST_STATIC void LLIST_APPEND (struct LLIST_LLIST * self, LLIST_DATA_TYPE val);
LLIST_STATIC void LLIST_MAP (struct LLIST_LLIST * self, LLIST_DATA_TYPE (* f) (LLIST_DATA_TYPE val));

/*==========================================================
 * Function definitions
==========================================================*/

/**
 * @brief Creates a new empty List
 * @returns The new List
 */
LLIST_STATIC struct LLIST_LLIST LLIST_NEW (void)
{
    return (struct LLIST_LLIST) {
        .length = 0,
        .head = NULL,
        .last = NULL,
    };
}

/**
 * @brief Free @a self
 * @param self The List to free
 */
LLIST_STATIC void LLIST_FREE (struct LLIST_LLIST * self)
{
    /*
     * Do nothing if `self == NULL` or
     * `length == 0` and `head == NULL` and `last == NULL`
     */
    if (self == NULL ||
        (self->length == 0 && self->head == NULL && self->last == NULL))
        return;

    /*
     * Getting here means the list is not empty.
     * If the list is not empty, all of the following must be true.
     * If they're not all true, the list is corrupt.
     */
    assert(self->length > 0);
    assert(self->head != NULL);
    assert(self->last != NULL);

    /*
     * Simple linear free
     *
     * 1. Save head;
     * 2. Move head forward;
     * 3. Free saved head;
     * 4. If `head != last` repeat;
     */
    do {
        struct LLIST_NODE * tmp = self->head;
        self->head = self->head->next;
        free(tmp);
    } while (self->head != self->last);

    self->length = 0;
    self->head = NULL;
    self->last = NULL;
}

/**
 * @brief Return the length of @a self
 * @param self The List
 * @returns The length of @a self
 */
LLIST_STATIC size_t LLIST_LEN (const struct LLIST_LLIST * self)
{
    assert(self != NULL);
    return self->length;
}

/**
 * @brief Return the element in the @a nth index of @a self
 * @param self The List
 * @param nth The index
 * @returns The @a nth element of @a self
 */
LLIST_STATIC LLIST_DATA_TYPE LLIST_NTH (const struct LLIST_LLIST * self, size_t nth)
{
    assert(self != NULL);
    assert(nth < self->length);
    assert(self->head != NULL);
    assert(self->last != NULL);

#if 0
    /* Is `nth` closer to the last node? */
    bool cond = nth > (self->length / 2);

    size_t idx = (cond) ?
        self->length - 1 - nth :
        nth;

    struct LLIST_NODE * node = (cond) ?
        self->last :
        self->head ;
    for (idx++; idx > 0; idx--)
        node = (cond) ?
            /* Move forward */
            node->next :
            /* Move backwards */
            node->prev;
#else
    struct LLIST_NODE * node = self->head;
    for (nth++; nth > 0; nth--)
        node = node->next;
#endif

    return node->val;
}

/**
 * @brief Create a new Node
 * @param val The value
 * @param prev The previous Node in the List
 * @param next The next Node in the List
 * @returns The new Node
 */
LLIST_STATIC struct LLIST_NODE * LLIST_NODE_NEW (LLIST_DATA_TYPE val, struct LLIST_NODE * prev, struct LLIST_NODE * next)
{
    struct LLIST_NODE * ret = malloc(sizeof(struct LLIST_NODE));

    if (ret != NULL) {
        *ret = (struct LLIST_NODE) {
            .val = val,
            .prev = (prev == NULL) ?
                ret :
                prev,
            .next = (next == NULL) ?
                ret :
                next,
        };
    }

    return ret;
}

/**
 * @brief Add an element to an empty List
 * @param self The List
 * @param val The val to add
 */
LLIST_STATIC void LLIST_EMPTY_ADD (struct LLIST_LLIST * self, LLIST_DATA_TYPE val)
{
    assert(self != NULL);
    assert(self->length == 0);
    assert(self->head == NULL);
    assert(self->last == NULL);

    self->head = self->last = LLIST_NODE_NEW(val, NULL, NULL);
    self->length = (self->head != NULL) ? 1 : 0;
}

/**
 * @brief Add an element to a non-empty List
 * @param self The List
 * @param val The val to add
 * @returns A pointer to the new Node
 */
LLIST_STATIC struct LLIST_NODE * LLIST_NON_EMPTY_ADD (struct LLIST_LLIST * self, LLIST_DATA_TYPE val)
{
    assert(self != NULL);
    assert(self->length > 0);
    assert(self->head != NULL);
    assert(self->last != NULL);

    struct LLIST_NODE * ret = LLIST_NODE_NEW(val, self->last, self->head);

    if (ret != NULL) {
        self->length++;
        self->head->prev = self->last->next = ret;
    }

    return ret;
}

LLIST_STATIC void LLIST_CONS (struct LLIST_LLIST * self, LLIST_DATA_TYPE val)
{
    assert(self != NULL);
    if (self->length == 0) {
        LLIST_EMPTY_ADD(self, val);
    } else {
        struct LLIST_NODE * new = LLIST_NON_EMPTY_ADD(self, val);
        self->head = (new == NULL) ?
            self->head :
            new;
    }
}

LLIST_STATIC void LLIST_APPEND (struct LLIST_LLIST * self, LLIST_DATA_TYPE val)
{
    assert(self != NULL);
    if (self->length == 0) {
        LLIST_EMPTY_ADD(self, val);
    } else {
        struct LLIST_NODE * new = LLIST_NON_EMPTY_ADD(self, val);
        self->last = (new == NULL) ?
            self->last :
            new;
    }
}

LLIST_STATIC void LLIST_MAP (struct LLIST_LLIST * self, LLIST_DATA_TYPE (* f) (LLIST_DATA_TYPE val))
{
    assert(self != NULL);
    assert(f != NULL);

    if (self->length > 0) {
        struct LLIST_NODE * node = self->head;

        do {
            node->val = f(node->val);
            node = node->next;
        } while (node != self->last);
    }
}

/*==========================================================
 * Clean up
==========================================================*/

/*
 * Functions
 */
#undef LLIST_NEW
#undef LLIST_FREE
#undef LLIST_LEN
#undef LLIST_NTH
#undef LLIST_NODE_NEW
#undef LLIST_EMPTY_ADD
#undef LLIST_NON_EMPTY_ADD
#undef LLIST_CONS
#undef LLIST_APPEND

/*
 * Other
 */
#undef LLIST_DATA_TYPE
#undef LLIST_DATA_TYPE_EQ
#undef LLIST_LLIST
#undef LLIST_NODE
#undef LLIST_PREFIX
#undef LLIST_STATIC

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
