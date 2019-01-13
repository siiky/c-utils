#ifndef _BS_H
#define _BS_H

#include <stdbool.h>
#include <stdio.h>

struct bs {
    struct byte * bytes;
    unsigned      nbits;
};

bool bs_eprint   (const struct bs * self);
bool bs_eq       (const struct bs * self, const struct bs * other);
bool bs_flip     (struct bs * self, unsigned nth);
bool bs_flip_all (struct bs * self);
bool bs_fprint   (const struct bs * self, FILE * outf);
bool bs_free     (struct bs * self);
bool bs_get      (const struct bs * self, unsigned nth);
bool bs_init     (struct bs * self, unsigned nbits);
bool bs_print    (const struct bs * self);
bool bs_set      (struct bs * self, unsigned nth, bool val);
bool bs_set_all  (struct bs * self, bool val);

#ifdef BS_CFG_IMPLEMENTATION

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct byte {
    uint8_t b0 : 1;
    uint8_t b1 : 1;
    uint8_t b2 : 1;
    uint8_t b3 : 1;
    uint8_t b4 : 1;
    uint8_t b5 : 1;
    uint8_t b6 : 1;
    uint8_t b7 : 1;
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

static inline bool _bs_clean (struct bs * self)
{
    *self = (struct bs) {0};
    return true;
}

#define bs_valid(bs)        if (!_bs_valid(bs)) return false
#define bs_nbits2idx(nbits) (nbits / 8)
#define bs_nbits2len(nbits) ((nbits / 8) + ((nbits % 8 != 0) ? 1 : 0))

bool bs_eprint (const struct bs * self)
{
    return bs_fprint(self, stderr);
}

bool bs_eq (const struct bs * self, const struct bs * other)
{
    if (self == other)
        return true;
    if (self == NULL || other == NULL)
        return false;

    bool ret = self->nbits == other->nbits;
    for (unsigned n = 0; ret && n < self->nbits; n++)
        ret = bs_get(self, n) == bs_get(other, n);

    return ret;
}

bool bs_flip (struct bs * self, unsigned nth)
{
    bs_valid(self);
    unsigned idx = bs_nbits2idx(nth);
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
            free(self->bytes);
        _bs_clean(self);
    }
    return true;
}

bool bs_get (const struct bs * self, unsigned nth)
{
    bs_valid(self);
    unsigned idx = bs_nbits2idx(nth);
    return nth < self->nbits
        && idx < bs_nbits2len(self->nbits)
        && _bs_byte_get(self->bytes + idx, nth % 8);
}

bool bs_init (struct bs * self, unsigned nbits)
{
    if (self == NULL)
        return false;
    _bs_clean(self);

    unsigned len = bs_nbits2len(nbits);
    size_t size = len * sizeof(struct byte);
    self->bytes = malloc(size);
    bool ret = self->bytes != NULL;

    if (ret) {
        memset(self->bytes, 0, size);
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
    unsigned idx = bs_nbits2idx(nth);
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
#endif /* BS_CFG_IMPLEMENTATION */
#undef BS_CFG_IMPLEMENTATION
#endif /* _BS_H */
