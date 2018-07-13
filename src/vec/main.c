#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Example with a vec of type char */
#define CHAR

/* Example with a vec of type int */
#define INT

#include "vec.h"

#define NELEMS 100000

int main (void)
{
    srand(time(NULL));

# ifdef INT
    puts("INT");
    struct ivec ivec = {0};
    if (vec_init(&ivec, NELEMS)) {
        printf("len: %zu\tcap: %zu\n", vec_len(&ivec), vec_capacity(&ivec));

        while (!vec_is_empty(&ivec))
            vec_swap_remove(&ivec, 0);

        vec_print(&ivec);
        ivec = vec_free(ivec);
    }
# endif /* INT */

# ifdef CHAR
    puts("CHAR");
    struct cvec cvec = {0};
    if (vec_init(&cvec, NELEMS)) {
        printf("len: %zu\tcap: %zu\n", vec_len(&cvec), vec_capacity(&cvec));

        while (!vec_is_empty(&cvec))
            vec_swap_remove(&cvec, 0);

        vec_print(&cvec);
        cvec = vec_free(cvec);
    }
# endif /* CHAR */

    return 0;
}
