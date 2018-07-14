#include "vec/qc.h"

#include <stdlib.h>

int main (void)
{
    bool (*tests[]) (void) = {
        qc_vec_test_all,
    };

    size_t ntests = sizeof(tests) / sizeof(*tests);

    bool succ = true;

    for (size_t i = 0; i < ntests; i++)
        succ = tests[i]() && succ;

    return (succ) ?
        EXIT_SUCCESS:
        EXIT_FAILURE;
}
