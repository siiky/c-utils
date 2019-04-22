/* bs - v2019.01.19-0
 *
 * A BitSet type inspired by
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *
 * The most up to date version of this file can be found at
 * `include/utils/bs.h` on [siiky/c-utils](https://github.com/siiky/c-utils)
 * More usage examples can be found at `examples/bs` on the link above
 */
#ifndef _BS_H
#define _BS_H

#include <stdbool.h>
#include <stdio.h>

struct byte;
struct bs {
    struct byte * bytes;
    unsigned      nbits;
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

#ifndef BS_CFG_MALLOC
# define BS_CFG_MALLOC malloc
#endif /* BS_CFG_MALLOC */

#ifndef BS_CFG_FREE
# define BS_CFG_FREE free
#endif /* BS_CFG_FREE */

#include <stdlib.h>
#include <string.h>

struct byte {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
};

static inline bool _bs_byte_get (const struct byte * self, unsigned nth)
{
    switch (nth) {
        case 0: return self->b0;
        case 1: return self->b1;
        case 2: return self->b2;
        case 3: return self->b3;
        case 4: return self->b4;
        case 5: return self->b5;
        case 6: return self->b6;
        case 7: return self->b7;
        default: return false;
    }
}

static inline bool _bs_byte_set (struct byte * self, unsigned nth, bool val)
{
    switch (nth) {
        case 0: self->b0 = val; break;
        case 1: self->b1 = val; break;
        case 2: self->b2 = val; break;
        case 3: self->b3 = val; break;
        case 4: self->b4 = val; break;
        case 5: self->b5 = val; break;
        case 6: self->b6 = val; break;
        case 7: self->b7 = val; break;
        default: return false;
    }
    return true;
}

static inline bool _bs_byte_flip (struct byte * self, unsigned nth)
{
    return _bs_byte_set(self, nth, !_bs_byte_get(self, nth));
}

static inline bool _bs_valid (const struct bs * self)
{
    return self != NULL
        && ((self->nbits == 0 && self->bytes == NULL)
                || (self->nbits > 0 && self->bytes != NULL));
}

#define bs_nbits2len(nbits) ((nbits / 8) + ((nbits % 8 != 0) ? 1 : 0))
#define bs_nth2idx(nbits)   (nbits / 8)
#define bs_valid(bs)        if (!_bs_valid(bs)) return false

bool bs_clone (const struct bs * restrict self, struct bs * restrict other)
{
    if (self == NULL || other == NULL)
        return false;

    unsigned len = bs_nbits2len(self->nbits);
    size_t size = len * sizeof(struct byte);
    void * bytes = BS_CFG_MALLOC(size);
    bool ret = bytes != NULL;

    if (ret) {
        memcpy(bytes, self->bytes, size);
        other->bytes = bytes;
        other->nbits = self->nbits;
    }

    return ret;
}

bool bs_eprint (const struct bs * self)
{
    return bs_fprint(self, stderr);
}

bool bs_flip (struct bs * self, unsigned nth)
{
    bs_valid(self);
    unsigned idx = bs_nth2idx(nth);
    return nth < self->nbits
        && idx < bs_nbits2len(self->nbits)
        && _bs_byte_flip(self->bytes + idx, nth % 8);
}

bool bs_flip_all (struct bs * self)
{
    bool ret = self != NULL;
    for (unsigned n = 0; ret && n < self->nbits; n++)
        ret = bs_flip(self, n);
    return ret;
}

bool bs_fprint (const struct bs * self, FILE * outf)
{
    bs_valid(self);
    if (outf == NULL)
        return false;

    for (unsigned i = 0; i < self->nbits; i++)
        fprintf(outf, "bit %u = %u\n", i, bs_get(self, i));

    return true;
}

bool bs_free (struct bs * self)
{
    if (self != NULL) {
        if (self->bytes != NULL)
            BS_CFG_FREE(self->bytes);
        *self = (struct bs) {0};
    }
    return true;
}

bool bs_get (const struct bs * self, unsigned nth)
{
    bs_valid(self);
    unsigned idx = bs_nth2idx(nth);
    return nth < self->nbits
        && idx < bs_nbits2len(self->nbits)
        && _bs_byte_get(self->bytes + idx, nth % 8);
}

bool bs_init (struct bs * self, unsigned nbits)
{
    if (self == NULL)
        return false;

    unsigned len = bs_nbits2len(nbits);
    size_t size = len * sizeof(struct byte);
    void * bytes = BS_CFG_MALLOC(size);
    bool ret = bytes != NULL;

    if (ret) {
        memset(bytes, 0, size);
        self->bytes = bytes;
        self->nbits = nbits;
    }

    return ret;
}

bool bs_print (const struct bs * self)
{
    return bs_fprint(self, stdout);
}

bool bs_set (struct bs * self, unsigned nth, bool val)
{
    bs_valid(self);
    unsigned idx = bs_nth2idx(nth);
    return nth < self->nbits
        && idx < bs_nbits2len(self->nbits)
        && _bs_byte_set(self->bytes + idx, nth % 8, val);
}

bool bs_set_all (struct bs * self, bool val)
{
    bool ret = self != NULL;
    for (unsigned n = 0; ret && n < self->nbits; n++)
        ret = bs_set(self, n, val);
    return ret;
}

int bs_cmp (const struct bs * restrict self, const struct bs * restrict other)
{
    if (self == other)              return 0;
    if (self == NULL)               return -1;
    if (other == NULL)              return 1;
    if (self->nbits < other->nbits) return -1;
    if (self->nbits > other->nbits) return 1;
    size_t size = bs_nbits2len(self->nbits) * sizeof(struct byte);
    return memcmp(self->bytes, other->bytes, size);
}

unsigned bs_hash (const struct bs * self)
{
    unsigned ret = 0;
    for (unsigned i = 0; i < self->nbits; i++)
        if (bs_get(self, i))
            ret += (1U << i);
    return ret;
}

#undef bs_nbits2len
#undef bs_nth2idx
#undef bs_valid

#undef BS_CFG_FREE
#undef BS_CFG_IMPLEMENTATION
#undef BS_CFG_MALLOC
#endif /* BS_CFG_IMPLEMENTATION */
