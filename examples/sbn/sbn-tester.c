#define SBN_CFG_IMPLEMENTATION
#include <utils/sbn.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define EXIT_OK 0
#define EXIT_UNIMPLEMENTED 1
#define EXIT_OPFAIL 2
#define EXIT_BADRESULT 3

#define streq(s1, s2) (strcmp((s1), (s2)) == 0)

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
    return streq(astr, s);
}

int main (int argc, char ** argv)
{
    assert(argc == 5);

    const char * opstr = argv[1];
    const char * astr = argv[2];
    const char * bstr = argv[3];
    const char * expected = argv[4];

    bool (*op) (struct sbn *, const struct sbn *, const struct sbn *) =
        streq(opstr, "+") ? sbn_add_u :
        streq(opstr, "-") ? sbn_sub_u :
        streq(opstr, "*") ? sbn_mul_u :
        streq(opstr, "/") ? unimplemented :
        NULL;

    assert(op != NULL);
    if (op == unimplemented) return EXIT_UNIMPLEMENTED;

    struct sbn a[1] = {0};
    struct sbn b[1] = {0};
    struct sbn r[1] = {0};

    assert(sbn_from_str_16(a, strlen(astr), astr));
    assert(ensure_equal_string(a, astr));

    assert(sbn_from_str_16(b, strlen(bstr), bstr));
    assert(ensure_equal_string(b, bstr));

    if (!op(r, a, b)) return EXIT_OPFAIL;

    char * rstr = sbn_to_str_16(r);
    assert(rstr != NULL);
    //fprintf(stderr, "result: '%s'\n", rstr);

    return streq(rstr, expected) ? EXIT_OK : EXIT_BADRESULT;
}
