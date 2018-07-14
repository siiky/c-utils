#include <theft.h>

#include "defs.h"
#include "vec.h"

#include "push.h"

bool qc_vec_test_all (void)
{
    bool (*tests[]) (void) = {
        qc_vec_push_test,
    };

    size_t ntests = sizeof(tests) / sizeof(*tests);

    bool ret = true;

    for (size_t i = 0; i < ntests; i++)
        ret = tests[i]() && ret;

    return ret;
}
