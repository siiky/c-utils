#include "defs.h"
#include "vec.h"

#include "push.h"

static enum theft_trial_res qc_vec_push_prop (struct theft * t, void * arg1)
{
    struct vec * vec = (struct vec *) arg1;
    unsigned int elem = (unsigned int) theft_random_bits(t, 32);

    size_t pre_len = vec_len(vec);

    bool res = vec_push(vec, elem);

    size_t pos_len = vec_len(vec);

    return ((res) ?
            (pos_len == (pre_len + 1)):
            (pos_len == pre_len)) ?
        THEFT_TRIAL_PASS:
        THEFT_TRIAL_FAIL;
}

bool qc_vec_push_test (void)
{
    struct theft_run_config cfg = {
        .name = __func__,
        .prop1 = qc_vec_push_prop,
        .type_info = {
            &qc_vec_info,
        },
        .seed = theft_seed_of_time(),
    };

    return theft_run(&cfg) == THEFT_RUN_PASS;
}
