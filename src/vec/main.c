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
    struct int_vec ivec = vec_init(&ivec, 100000);
    vec_print(&ivec);
    vec_free(&ivec, NULL);
# endif /* INT */

# ifdef CHAR
    puts("CHAR");
    struct char_vec cvec = vec_init(&cvec, 100000);
    vec_print(&cvec);
    vec_free(&cvec, NULL);
# endif /* CHAR */

    return 0;
}
