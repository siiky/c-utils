#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Example with a vec of type char */
#define CHAR

/* Example with a vec of type int */
#define INT

#include "vec.h"

int main (void)
{
    srand(time(NULL));

# ifdef INT
    puts("INT");
    struct int_vec ivec;
    if (vec_init(&ivec, 1000000)) {
        printf("len: %zu\tcap: %zu\n", vec_len(&ivec), vec_capacity(&ivec));
        vec_print(&ivec);
        ivec = vec_free(ivec);
    }
# endif /* INT */

# ifdef CHAR
    puts("CHAR");
    struct char_vec cvec;
    if (vec_init(&cvec, 1000000)) {
        printf("len: %zu\tcap: %zu\n", vec_len(&cvec), vec_capacity(&cvec));
        vec_print(&cvec);
        cvec = vec_free(cvec);
    }
# endif /* CHAR */

    return 0;
}
