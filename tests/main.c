#include "map/qc.h"
#include "vec/qc.h"

#include <unused.h>

#include <search.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int usage (const char * cmd)
{
    return printf(
            "Usage:\n"
            "\t%s\n"
            "\t\tTest all modules\n"
            "\t%s x MOD...\n"
            "\t\tTest all modules but the ones specified\n"
            "\t%s o MOD...\n"
            "\t\tTest only the specified modules\n",
            cmd, cmd, cmd), EXIT_FAILURE;
}

typedef bool (*test_func) (void);
struct mod {
    char * module;
    test_func test;
};

#define fill_table(N, KEY, DATA) do { \
    if (hcreate(N) == 0) return false;                           \
    for (size_t i = 0; i < (N); i++) {                           \
        ENTRY e = {                                              \
            .key = (KEY),                                        \
            .data = (DATA),                                      \
        };                                                       \
        if (hsearch(e, ENTER) == NULL) return hdestroy(), false; \
    } \
} while (0)

static bool test_except (size_t N, const struct mod tests[N], size_t argc, char * argv[argc])
{
    fill_table(argc, argv[i], NULL);

    bool succ = true;
    for (size_t i = 0; i < N; i++) {
        ENTRY e = { .key = tests[i].module, };

        if (hsearch(e, FIND) == NULL)
            succ = tests[i].test() && succ;
        else
            fprintf(stderr, "Skipping '%s'\n", tests[i].module);
    }

    return hdestroy(), succ;
}

static bool test_only (size_t N, const struct mod tests[N], size_t argc, char * argv[argc])
{
    fill_table(N, tests[i].module, tests[i].test);

    bool succ = true;
    for (size_t i = 0; i < argc; i++) {
        ENTRY e = { .key = argv[i], };
        ENTRY * ep = hsearch(e, FIND);

        if (ep != NULL) {
            test_func test = ep->data;
            succ = test() && succ;
        } else {
            fprintf(stderr, "Module '%s' not found\n", argv[i]);
        }
    }

    return hdestroy(), succ;
}

static bool test_all (size_t N, const struct mod tests[N], size_t argc, char * argv[argc])
{
    UNUSED(argc);
    UNUSED(argv);

    bool succ = true;
    for (size_t i = 0; i < N; i++)
        succ = tests[i].test() && succ;
    return succ;
}

#define _(MOD) { \
    .module = # MOD,                 \
    .test = qc_ ## MOD ## _test_all, \
}
static const struct mod TESTS[] = {
    _(vec),
    _(map),
};
#undef _

static const size_t NTESTS = sizeof(TESTS) / sizeof(TESTS[0]);

#define match(ACTION, CMD) \
    (strcmp(argv[1], CMD) == 0) ? ACTION

int main (int _argc, char * argv[_argc])
{
    size_t argc = (size_t) ((_argc < 0) ? 1 : _argc);
    bool (*action) (size_t N, const struct mod tests[N], size_t argc, char * argv[argc]) = test_all;

    if (argc == 2) {
        action = NULL;
    } else if (argc > 2) {
        action =
            match(test_except, "x"): /* tests x mod+ */
            match(test_only,   "o"): /* tests o mod+ */
            NULL;

        argc -= 2;
        argv += 2;
    }

    return (action == NULL) ?
        usage(*argv):
        (action(NTESTS, TESTS, argc, argv)) ?
        EXIT_SUCCESS:
        EXIT_FAILURE;
}
