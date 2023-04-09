#define SBN_CFG_IMPLEMENTATION
#include <utils/sbn.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define EXIT_OK 0
#define EXIT_UNIMPLEMENTED 1
#define EXIT_OPFAIL 2
#define EXIT_BADRESULT 3

bool unimplemented (struct sbn * r, const struct sbn * a, const struct sbn * b)
{
    (void) r;
    (void) a;
    (void) b;
    return false;
}

bool ensure_equal_string (const struct sbn * a, const char * s)
{
    char * astr = sbn_to_str_16(a);
    assert(astr != NULL);
    return strcmp(astr, s) == 0;
}

int main (int argc, char ** argv)
{
    assert(argc == 5);

    const char * opstr = argv[1];
    const char * astr = argv[2];
    const char * bstr = argv[3];
    const char * expected = argv[4];

    bool (*op) (struct sbn *, const struct sbn *, const struct sbn *) =
        strcmp(opstr, "+") == 0 ? sbn_add_u :
        strcmp(opstr, "-") == 0 ? sbn_sub_u :
        strcmp(opstr, "*") == 0 ? sbn_mul_u :
        strcmp(opstr, "/") == 0 ? unimplemented :
        NULL;

    assert(op != NULL);
    if (op == unimplemented) return EXIT_UNIMPLEMENTED;

    struct sbn a[1] = {{0}};
    struct sbn b[1] = {{0}};
    struct sbn r[1] = {{0}};

    assert(sbn_from_str_16(a, strlen(astr), astr));
    assert(ensure_equal_string(a, astr));

    assert(sbn_from_str_16(b, strlen(bstr), bstr));
    assert(ensure_equal_string(b, bstr));

    if (!op(r, a, b)) return EXIT_OPFAIL;

    char * rstr = sbn_to_str_16(r);
    assert(rstr != NULL);

    return strcmp(rstr, expected) == 0 ? EXIT_OK : EXIT_BADRESULT;
}
