/* bs - v2019.05.01-0
 *
 * A BitSet type inspired by
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *
 * The most up to date version of this file can be found at
 * `include/utils/bs.h` on [siiky/c-utils](https://git.sr.ht/~siiky/c-utils)
 * More usage examples can be found at `examples/bs` on the link above
 */
#ifndef _BS_H
#define _BS_H

/*
 * <stdbool.h>
 *  bool
 *  false
 *  true
 *
 * <stdio.h>
 *  FILE
 *  fprintf()
 *  printf()
 *  stderr
 *  stdout
 */
#include <stdbool.h>
#include <stdio.h>

typedef long long unsigned segment;
struct bs {
    segment * segs;
    unsigned  nbits;
};

bool     bs_clone    (const struct bs * restrict self, struct bs * restrict other);
bool     bs_eprint   (const struct bs * self);
bool     bs_flip     (struct bs * self, unsigned nth);
bool     bs_flip_all (struct bs * self);
bool     bs_fprint   (const struct bs * self, FILE * outf);
bool     bs_free     (struct bs * self);
bool     bs_get      (const struct bs * self, unsigned nth);
bool     bs_init     (struct bs * self, unsigned nbits);
bool     bs_print    (const struct bs * self);
bool     bs_set      (struct bs * self, unsigned nth, bool val);
bool     bs_set_all  (struct bs * self, bool val);
int      bs_cmp      (const struct bs * restrict self, const struct bs * restrict other);
unsigned bs_hash     (const struct bs * self);

#endif /* _BS_H */

#ifdef BS_CFG_IMPLEMENTATION

#ifndef BS_CFG_CALLOC
# define BS_CFG_CALLOC calloc
#endif /* BS_CFG_CALLOC */

#ifndef BS_CFG_FREE
# define BS_CFG_FREE free
#endif /* BS_CFG_FREE */

/*
 * <limits.h>
 *  CHAR_BIT
 *
 * <stdlib.h>
 *  calloc()
 *  free()
 *
 * <string.h>
 *  memcmp()
 *  memcpy()
 *  memset()
 */
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/* Assume no padding bits */
#define bs_bits_per_seg (CHAR_BIT * sizeof(segment))

static inline bool _bs_valid (const struct bs * self)
{
    return self != NULL
        && ((self->nbits > 0) ?
                self->segs != NULL:
                self->segs == NULL);
}

static inline unsigned bs_nth2segidx (unsigned nth)
{ return nth / bs_bits_per_seg; }

static inline unsigned char bs_nth2bitidx (unsigned nth)
{ return nth % bs_bits_per_seg; }

static inline unsigned bs_nbits2arrlen (unsigned nbits)
{ return bs_nth2segidx(nbits) + ((bs_nth2bitidx(nbits)) ? 1 : 0); }

bool bs_clone (const struct bs * restrict self, struct bs * restrict other)
{
    return _bs_valid(self)
        && other != NULL
        && bs_init(other, self->nbits)
        && (memcpy(other->segs, self->segs,
                    bs_nbits2arrlen(self->nbits) * sizeof(segment)),
                true);
}

bool bs_eprint (const struct bs * self)
{ return bs_fprint(self, stderr); }

bool bs_flip (struct bs * self, unsigned nth)
{
    unsigned segidx = bs_nth2segidx(nth);
    unsigned char bitidx = bs_nth2bitidx(nth);
    return _bs_valid(self)
        && nth < self->nbits
        && bitidx < bs_bits_per_seg
        && ((self->segs[segidx] ^= 1UL << bitidx), true);
}

bool bs_flip_all (struct bs * self)
{
    if (!_bs_valid(self)) return false;
    unsigned len = bs_nbits2arrlen(self->nbits);
    for (unsigned i = 0; i < len; i++)
        self->segs[i] ^= ~0UL;
    return true;
}

bool bs_fprint (const struct bs * self, FILE * outf)
{
    if (!_bs_valid(self) || outf == NULL) return false;
    unsigned len = bs_nbits2arrlen(self->nbits);
    for (unsigned i = 0; i < len; i++)
        fprintf(outf, "%llX", self->segs[i]);
    return true;
}

bool bs_free (struct bs * self)
{
    if (self != NULL) {
        if (self->segs != NULL)
            BS_CFG_FREE(self->segs);
        *self = (struct bs) {0};
    }
    return true;
}

bool bs_get (const struct bs * self, unsigned nth)
{
    unsigned segidx = bs_nth2segidx(nth);
    unsigned char bitidx = bs_nth2bitidx(nth);
    return _bs_valid(self)
        && nth < self->nbits
        && bitidx < bs_bits_per_seg
        && (self->segs[segidx] & (1UL << bitidx));
}

bool bs_init (struct bs * self, unsigned nbits)
{
    if (self == NULL) return false;
    unsigned len = bs_nbits2arrlen(nbits);
    segment * segs = BS_CFG_CALLOC(len, sizeof(segment));
    bool ret = segs != NULL;
    if (ret) {
        self->segs = segs;
        self->nbits = nbits;
    }
    return ret;
}

bool bs_print (const struct bs * self)
{ return bs_fprint(self, stdout); }

bool bs_set (struct bs * self, unsigned nth, bool _val)
{
#define seg self->segs[segidx]
    unsigned segidx = bs_nth2segidx(nth);
    unsigned char bitidx = bs_nth2bitidx(nth);
    segment m = 1UL << bitidx;
    segment val = -((segment) _val);
    return _bs_valid(self)
        && nth < self->nbits
        && bitidx < bs_bits_per_seg
        && ((seg = (seg & ~m) | (val & m)), true);
#undef seg
}

bool bs_set_all (struct bs * self, bool val)
{
    return _bs_valid(self)
        && (memset(self->segs, (val) ? ~0L : 0L,
                    bs_nbits2arrlen(self->nbits) * sizeof(segment)),
                true);
}

int bs_cmp (const struct bs * restrict self, const struct bs * restrict other)
{
    if (self == other)              return 0;
    if (self == NULL)               return -1;
    if (other == NULL)              return 1;
    if (self->nbits < other->nbits) return -1;
    if (self->nbits > other->nbits) return 1;
    return memcmp(self->segs, other->segs,
            bs_nbits2arrlen(self->nbits) * sizeof(segment));
}

unsigned bs_hash (const struct bs * self)
{
    if (!_bs_valid(self)) return 0;
    unsigned ret = 0;
    unsigned len = bs_nbits2arrlen(self->nbits);
    for (unsigned segidx = 0; segidx < len; segidx++)
        for (unsigned bidx = 0; bidx < sizeof(segment); bidx++)
            ret ^= ((unsigned char *) &self->segs[segidx])[bidx];
    return ret;
}

#undef BS_CFG_FREE
#undef BS_CFG_IMPLEMENTATION
#undef BS_CFG_CALLOC
#endif /* BS_CFG_IMPLEMENTATION */
