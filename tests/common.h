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
                __VA_ARGS__,                      \
            },                                    \
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
            __VA_ARGS__,                                \
        };                                              \
        size_t ntests = sizeof(tests) / sizeof(*tests); \
        bool ret = true;                                \
        for (size_t i = 0; i < ntests; i++)             \
            ret = tests[i]() && ret;                    \
        return ret;                                     \
    } bool fname (void)

#include <theft.h>
#include "common/int.h"

#endif /* _COMMON_H */
