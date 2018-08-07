#ifndef _COMMON_H
#define _COMMON_H

/**
 * @brief Create a function to test a proposition
 * @param fname The function name
 * @param propf The field (prop[1-7])
 * @param prop The proposition to test
 * @param ... The generators (in order)
 */
#define QC_MKTEST(fname, propf, prop, ...)        \
    static bool fname (void) {                    \
        struct theft_run_config cfg = {           \
            .name = __func__,                     \
            .propf = prop,                        \
            .type_info = {                        \
                __VA_ARGS__                       \
            },                                    \
            .trials = 200,                        \
            .seed = theft_seed_of_time(),         \
        };                                        \
        return theft_run(&cfg) == THEFT_RUN_PASS; \
    } bool fname (void)

/**
 * @brief Create a function to run a group of tests
 * @param fname The function name
 * @param ... The tests to run
 */
#define QC_MKTEST_ALL(fname, ...)                       \
    bool fname (void) {                                 \
        bool (*tests[]) (void) = {                      \
            __VA_ARGS__                                 \
        };                                              \
        size_t ntests = sizeof(tests) / sizeof(*tests); \
        bool ret = true;                                \
        for (size_t i = 0; i < ntests; i++)             \
            ret = tests[i]() && ret;                    \
        return ret;                                     \
    } bool fname (void)

#define QC_BOOL2TRIAL(succ) \
    ((succ) ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL)

#define QC_MKID(MOD, FUNC, TEST, TYPE) \
    qc_ ## MOD ## _ ## FUNC ## _ ## TEST ## _ ## TYPE

#define QC_MKID_ALL(MOD, FUNC) \
    qc_ ## MOD ## _ ## FUNC ## _test_all

#define QC_ARG2VAL(ARGN, TYPE) \
    (* (TYPE *) arg ## ARGN)

/**
 * @brief Cast and deref an arg to the correct type, and create a variable
 *        with that value
 * @param ARGN The argument number
 * @param TYPE The type of the variable to be created
 * @param VAR The name of the variable to be created
 */
#define QC_ARG2VAR(ARGN, TYPE, VAR) \
    TYPE VAR = QC_ARG2VAL(ARGN, TYPE)

#include <theft.h>

extern const struct theft_type_info qc_bool_info;
extern const struct theft_type_info qc_int_info;
extern const struct theft_type_info qc_size_t_info;

#include "common/int.h"

#endif /* _COMMON_H */
