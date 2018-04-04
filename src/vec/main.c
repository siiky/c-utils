#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Example with a vec of type char */
#define CHAR

/* Example with a vec of type int */
#define INT

#include "gen_vec.h"

int main (void)
{
    srand(time(NULL));

# ifdef INT
    puts("INT");
    struct int_vec ivec = vec_init(&ivec, 1000000);
    printf("len: %zu\tcap: %zu\n", vec_len(&ivec), vec_capacity(&ivec));
    vec_print(&ivec);
    vec_free(&ivec);
# endif /* INT */

# ifdef CHAR
    puts("CHAR");
    struct char_vec cvec = vec_init(&cvec, 1000000);
    printf("len: %zu\tcap: %zu\n", vec_len(&cvec), vec_capacity(&cvec));
    vec_print(&cvec);
    vec_free(&cvec);
# endif /* CHAR */

    return 0;
}
