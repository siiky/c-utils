#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Example with a Vec of type char */
#define CHAR

/* Example with a Vec of type int */
#define INT

# ifdef CHAR
#define VEC_DATA_TYPE char
#define VEC_IMPLEMENTATION
#define VEC_PREFIX char_
#include "vec.h"

struct char_Vec char_Vec_init (size_t capacity)
{
    struct char_Vec ret = char_with_capacity(capacity);
    for (size_t i = 1; i <= capacity; i++) {
#define R(I, X) ((rand() % (I)) + (X))
        char_push(&ret, R(i + capacity, 'a'));
        char_push(&ret, R(i + capacity, 'A'));
        char_push(&ret, R(i + capacity, '0'));
#undef R
    }
    return ret;
}

void char_Vec_print (struct char_Vec * vec)
{
    size_t len = char_len(vec);
    for (size_t i = 0; i < len; i++)
        printf("%c ", char_get_nth(vec, i));
    putchar('\n');
}
# endif /* CHAR */

# ifdef INT
#define VEC_STATIC
#define VEC_DATA_TYPE int
#define VEC_IMPLEMENTATION
#define VEC_PREFIX int_
#include "vec.h"

struct int_Vec int_Vec_init (size_t capacity)
{
    struct int_Vec ret = int_with_capacity(capacity);
    for (size_t i = 1; i <= capacity; i++) {
        int x = rand() % i; /* `Floating point exception` if `i` starts at 0 */
        int_push(&ret, x);
    }
    return ret;
}

void int_Vec_print (struct int_Vec * vec)
{
    size_t len = int_len(vec);
    for (size_t i = 0; i < len; i++)
        printf("%d ", int_get_nth(vec, i));
    putchar('\n');
}
# endif /* INT */

int main (void)
{
    srand(time(NULL));

# ifdef INT
    puts("INT");
    struct int_Vec ivec = int_Vec_init(10);
    int_Vec_print(&ivec);
    int_free(&ivec);
# endif /* INT */

# ifdef CHAR
    puts("CHAR");
    struct char_Vec cvec = char_Vec_init(10);
    char_Vec_print(&cvec);
    char_free(&cvec);
# endif /* CHAR */

    return 0;
}
