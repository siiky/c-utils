/* sbn - v2020.05.27-0
 *
 * A bignum type inspired by
 *  * Scheme
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *
 * The most up to date version of this file can be found at
 * `include/utils/sbn.h` on [siiky/c-utils](https://github.com/siiky/c-utils).
 *
 * It depends on `vec.h` which can be found next to this file in the link above.
 *
 * It can be configured to use different integer types for the digits, but
 * there can be only one implementation.
 */

# ifdef SBN_CFG_NO_STDINT

#  ifndef sbn_digit
#   error "Must define sbn_digit when using SBN_CFG_NO_STDINT"
#  endif /* sbn_digit */

#  ifndef sbn_digit_nbits
#   error "Must define sbn_digit_nbits when using SBN_CFG_NO_STDINT"
#  endif /* sbn_digit_nbits */

#  ifndef sbn_double_digit
#   error "Must define sbn_double_digit when using SBN_CFG_NO_STDINT"
#  endif /* sbn_double_digit */

#  ifndef sbn_double_digit_nbits
#   error "Must define sbn_double_digit_nbits when using SBN_CFG_NO_STDINT"
#  endif /* sbn_double_digit_nbits */

# else /* SBN_CFG_NO_STDINT */

/*
 * <stdint.h>
 *  uint32_t
 *  uint64_t
 */
#  include <stdint.h>

#  define sbn_digit              uint32_t
#  define sbn_digit_nbits        32U
#  define sbn_double_digit       uint64_t
#  define sbn_double_digit_nbits 64U
#  define sbn_nbits_diff         (sbn_double_digit_nbits - sbn_digit_nbits)

# endif /* SBN_CFG_NO_STDINT */

/*
 * <stdbool.h>
 *  bool
 *  false
 *  true
 *
 *  <stddef.h>
 *   size_t
 */
#include <stdbool.h>
#include <stddef.h>

/* I don't like it as an opaque structure, but I can't think of a workaround */
struct sbn;

bool         sbn_add_digit    (struct sbn * a, sbn_digit dig);
bool         sbn_free         (struct sbn * a);
bool         sbn_is_zero      (const struct sbn * a);
sbn_digit    sbn_nth_digit    (const struct sbn * a, size_t nth);
size_t       sbn_ndigits      (const struct sbn * a);
struct sbn * sbn_clone        (const struct sbn * a);
struct sbn * sbn_new          (void);
struct sbn * sbn_unsigned_add (struct sbn * a, struct sbn * b);

#ifdef SBN_CFG_IMPLEMENTATION

# define sbn_double_digit_upper_half(dig) ((sbn_digit) ((dig) >> sbn_nbits_diff))
# define sbn_double_digit_lower_half(dig) sbn_double_digit_upper_half((dig) << sbn_nbits_diff)

/*
 * NOTE(!!!): Must be exactly the same as the above, but with
 *            VEC_CFG_IMPLEMENTATION
 */
#define VEC_CFG_DATA_TYPE sbn_digit
#define VEC_CFG_VEC _sbn_digits_vec
#define VEC_CFG_IMPLEMENTATION
#include <utils/vec.h>

struct sbn {
	unsigned char is_negative : 1; /* UNUSED for now */
	struct _sbn_digits_vec digits[1];
};

static bool _sbn_digits_iter (struct sbn * a)
{ return a && _sbn_digits_vec_iter(a->digits); }

static bool _sbn_digits_itering (struct sbn * a)
{ return a && _sbn_digits_vec_itering(a->digits); }

static bool _sbn_digits_iter_next (struct sbn * a)
{ return a && _sbn_digits_vec_iter_next(a->digits); }

static size_t _sbn_digits_iter_idx (const struct sbn * a)
{ return a && _sbn_digits_vec_iter_idx(a->digits); }

static bool _sbn_reserve (struct sbn * a, size_t total_digs)
{ return a && _sbn_digits_vec_reserve(a->digits, total_digs); }

static bool _sbn_push_digit (struct sbn * a, sbn_digit dig)
{ return a && _sbn_digits_vec_push(a->digits, dig); }

static bool _sbn_append_digits (struct sbn * restrict a, const struct sbn * restrict b)
{
	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);

	if (!a || !b || !_sbn_reserve(a, andigs + bndigs))
		return false;

	sbn_digit * dest = a->digits->ptr + andigs;
	sbn_digit * src = b->digits->ptr;
	size_t n = bndigs * sizeof(sbn_digit);
	memcpy(dest, src, n);

	return true;
}

sbn_digit _sbn_add_digits (sbn_digit a, sbn_digit b, sbn_digit * carry)
{
	sbn_double_digit tmp = a + b + *carry;
	sbn_digit ret = sbn_double_digit_lower_half(tmp);
	*carry = sbn_double_digit_upper_half(tmp);
	return ret;
}

bool sbn_add_digit (struct sbn * a, sbn_digit dig)
{
	if (!a) return false;
	if (dig == 0) return true;

	sbn_digit carry = dig;

	while (carry > 0) {
		//dig = _sbn_add_digits(
	}

	return true;
}

bool sbn_free (struct sbn * a)
{
	if (a) {
		*a->digits = _sbn_digits_vec_free(*a->digits);
		a->is_negative = 0;
	}
	return true;
}

bool sbn_is_zero (const struct sbn * a)
{
	return a && sbn_ndigits(a) == 0;
}

sbn_digit sbn_nth_digit (const struct sbn * a, size_t nth)
{
	return _sbn_digits_vec_get_nth(a->digits, nth);
}

size_t sbn_ndigits (const struct sbn * a)
{
	return (a) ? _sbn_digits_vec_len(a->digits) : 0;
}

struct sbn * sbn_clone (const struct sbn * a)
{
	struct sbn * ret = (a) ? sbn_new() : NULL;

	if (ret) {
		_sbn_append_digits(ret, a);
		ret->is_negative = a->is_negative;
	}

	return ret;
}

struct sbn * sbn_new (void)
{
	return calloc(1, sizeof(struct sbn));
}

/*
 * TODO: Add numbers of different signs
 * TODO: `a` and `b` should be const
 *
 *  a +  b = a + b
 *  a + -b = a - b
 * -a +  b = b - a
 * -a + -b = -(a + b)
 *
 */
struct sbn * sbn_unsigned_add (struct sbn * a, struct sbn * b)
{
	if (!a && !b)
		return NULL;

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);

	/* if `b` is 0 then there's nothing to do */
	if (bndigs == 0)
		return sbn_clone(a);

	/* if `a` is 0 then we have to copy `b` to `a` */
	if (andigs == 0)
		return sbn_clone(b);

	/*
	 * Since we're going to clone one of `a` and `b`, might as well clone
	 * the one with more digits; from this `if` forward, `a` >= `b` in the
	 * number of digits
	 */
	if (andigs < bndigs) {
		{
			struct sbn * tmp = a;
			a = b;
			b = tmp;
		}

		{
			size_t tmp = andigs;
			andigs = bndigs;
			bndigs = tmp;
		}
	}

	struct sbn * ret = sbn_new();
	if (!ret) return NULL;
	if (!_sbn_reserve(ret, andigs)) return sbn_free(ret), NULL;

	sbn_digit carry = 0;

	/*
	 * Add common digits
	 */
	for (_sbn_digits_iter(a), _sbn_digits_iter(b);
			/* Checking `a` shouldn't be needed; just sanity */
			_sbn_digits_itering(a) && _sbn_digits_itering(b);
			_sbn_digits_iter_next(a), _sbn_digits_iter_next(b))
	{
		size_t ai = _sbn_digits_iter_idx(a);
		size_t bi = _sbn_digits_iter_idx(b);

		sbn_digit adig = sbn_nth_digit(a, ai);
		sbn_digit bdig = sbn_nth_digit(b, bi);

		if (!_sbn_push_digit(ret, _sbn_add_digits(adig, bdig, &carry)))
			return sbn_free(ret), NULL;
	}

	/*
	 * Add `a`'s remaining digits, if any; Since `a` >= `b` in the number
	 * of digits, if `a` is still iterating, then we still have digits to
	 * add
	 */
	for (; _sbn_digits_itering(a); _sbn_digits_iter_next(a)) {
		size_t ai = _sbn_digits_iter_idx(a);
		sbn_digit adig = sbn_nth_digit(a, ai);
		if (!_sbn_push_digit(ret, _sbn_add_digits(adig, 0, &carry)))
			return sbn_free(ret), NULL;
	}

	/*
	 * Finally, add the carry
	 */
	if (carry != 0 && !_sbn_push_digit(ret, carry))
		return sbn_free(ret), NULL;

	return ret;
}

#endif /* SBN_CFG_IMPLEMENTATION */
