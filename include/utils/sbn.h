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

# endif /* SBN_CFG_NO_STDINT */

# define sbn_nbits_diff                   (sbn_double_digit_nbits - sbn_digit_nbits)
# define sbn_double_digit_upper_half(dig) ((sbn_digit) ((dig) >> sbn_nbits_diff))
# define sbn_double_digit_lower_half(dig) sbn_double_digit_upper_half((dig) << sbn_nbits_diff)
# define sbn_twos_compl(dig)              (~(dig) + 1)
# define _sbn_min(a, b)                   (((a) < (b)) ? (a) : (b))
# define _sbn_max(a, b)                   (((a) > (b)) ? (a) : (b))

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

bool         sbn_add_digit_ud (struct sbn * a, sbn_digit dig);
bool         sbn_free         (struct sbn * a);
bool         sbn_is_negative  (const struct sbn * a);
bool         sbn_is_zero      (const struct sbn * a);
bool         sbn_negate       (struct sbn * a);
bool         sbn_set_sign     (struct sbn * a, bool is_negative);
int          sbn_sign         (const struct sbn * a);
sbn_digit    sbn_nth_digit    (const struct sbn * a, size_t nth);
size_t       sbn_ndigits      (const struct sbn * a);
struct sbn * sbn_add          (const struct sbn * a, const struct sbn * b);
struct sbn * sbn_add_digit_u  (struct sbn * a, sbn_digit dig);
struct sbn * sbn_add_u        (const struct sbn * a, const struct sbn * b);
struct sbn * sbn_clone        (const struct sbn * a);
struct sbn * sbn_new          (void);
struct sbn * sbn_sub_u        (const struct sbn * a, const struct sbn * b);

#ifdef SBN_CFG_IMPLEMENTATION

#define VEC_CFG_DATA_TYPE sbn_digit
#define VEC_CFG_VEC _sbn_digits_vec
#define VEC_CFG_IMPLEMENTATION
#include <utils/vec.h>

struct sbn {
	/** Is this SBN negative? */
	unsigned char is_negative : 1;

	/** The vector of digits */
	struct _sbn_digits_vec digits[1];
};

/**
 * @brief Reserve a total number of digits
 */
static bool _sbn_reserve (struct sbn * a, size_t total_digs)
{ return a && _sbn_digits_vec_reserve(a->digits, total_digs); }

/**
 * @brief Append a new digit
 */
static bool _sbn_push_digit (struct sbn * a, sbn_digit dig)
{ return a && _sbn_digits_vec_push(a->digits, dig); }

/**
 * @brief Set the @a nth digit to @a dig
 */
static bool _sbn_set_nth_digit (struct sbn * a, size_t i, sbn_digit dig)
{ return a && _sbn_digits_vec_set_nth(a->digits, i, dig); }

/**
 * @brief Append the digits of @a b to the end of @a a's digits
 */
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

/**
 * @brief Calculate the result and carry of adding two digits
 */
static sbn_digit _sbn_add_digits (sbn_digit _a, sbn_digit _b, sbn_digit * _carry)
{
	sbn_double_digit a = _a;
	sbn_double_digit b = _b;
	sbn_double_digit carry = *_carry;

	sbn_double_digit tmp = a + b + carry;
	*_carry = sbn_double_digit_upper_half(tmp);
	return sbn_double_digit_lower_half(tmp);
}

static sbn_digit _sbn_sub_digits (sbn_digit _a, sbn_digit _b, sbn_digit * _carry)
{
	sbn_double_digit a = _a;
	sbn_double_digit b = _b;
	sbn_double_digit carry = *_carry;

	sbn_double_digit tmp = a - b - carry;
	*_carry = sbn_double_digit_upper_half(tmp);
	return sbn_double_digit_lower_half(tmp);
}

/**
 * @brief Add a single digit to @a a, destructively, ignoring the sign
 */
bool sbn_add_digit_ud (struct sbn * a, sbn_digit dig)
{
	if (!a) return false;
	if (dig == 0) return true;

	sbn_digit carry = dig;
	size_t ndigs = sbn_ndigits(a);

	for (size_t i = 0; carry > 0 && i < ndigs; i++)
		_sbn_set_nth_digit(a, i, _sbn_add_digits(sbn_nth_digit(a, i), 0, &carry));

	return !carry || _sbn_push_digit(a, carry);
}

/**
 * @brief Free @a a
 */
bool sbn_free (struct sbn * a)
{
	if (a) {
		*a->digits = _sbn_digits_vec_free(*a->digits);
		a->is_negative = 0;
	}
	return true;
}

/**
 * @brief Is @a a negative?
 */
bool sbn_is_negative (const struct sbn * a)
{
	return a && a->is_negative;
}

/**
 * @brief Is @a a zero?
 */
bool sbn_is_zero (const struct sbn * a)
{
	return a && sbn_ndigits(a) == 0;
}

/**
 * @brief Negate @a a
 */
bool sbn_negate (struct sbn * a)
{
	return a && ((a->is_negative = !a->is_negative), true);
}

/**
 * @brief Set @a a's sign
 */
bool sbn_set_sign (struct sbn * a, bool is_negative)
{
	return a && ((a->is_negative = is_negative), true);
}

/**
 * @brief Get the sign of @a a
 */
int sbn_sign (const struct sbn * a)
{
	return (sbn_is_zero(a)) ?
		0:
		(sbn_is_negative(a)) ?
		-1:
		1;
}

/**
 * @brief Get the @a nth digit of @a a
 */
sbn_digit sbn_nth_digit (const struct sbn * a, size_t nth)
{
	return _sbn_digits_vec_get_nth(a->digits, nth);
}

/**
 * @brief Get the number of digits of @a a
 */
size_t sbn_ndigits (const struct sbn * a)
{
	return (a) ? _sbn_digits_vec_len(a->digits) : 0;
}

/**
 * @brief Add @a b to @a a
 *
 * TODO: Add numbers of different signs
 *
 * Same sign:
 *  a +  b = a + b
 * -a + -b = -(a + b)
 *
 * Different sign:
 *  a + -b = a - b
 * -a +  b = b - a
 */
struct sbn * sbn_add (const struct sbn * a, const struct sbn * b)
{
	struct sbn * ret = NULL;
	bool is_a_negative = sbn_is_negative(a);

	if (is_a_negative == sbn_is_negative(b)) {
		ret = sbn_add_u(a, b);
		sbn_set_sign(ret, is_a_negative);
	} else {
		ret = (is_a_negative) ?
			sbn_sub_u(b, a):
			sbn_sub_u(a, b);
		/* TODO: Set the right sign */
	}

	return ret;
}

/**
 * @brief Add a single digit to @a a, non-destructively, ignoring the sign
 */
struct sbn * sbn_add_digit_u (struct sbn * a, sbn_digit dig)
{
	struct sbn * ret = sbn_clone(a);
	return (ret && !sbn_add_digit_ud(ret, dig)) ?
		(sbn_free(ret), NULL):
		ret;
}

/**
 * @brief Add @a b to @a a, ignoring the sign
 */
struct sbn * sbn_add_u (const struct sbn * a, const struct sbn * b)
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
			const struct sbn * tmp = a;
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

	size_t minndigs = _sbn_min(andigs, bndigs);
	size_t maxndigs = _sbn_max(andigs, bndigs);
	sbn_digit carry = 0;

	/*
	 * Add common digits
	 */
	for (size_t i = 0; i < minndigs; i++) {
		sbn_digit adig = sbn_nth_digit(a, i);
		sbn_digit bdig = sbn_nth_digit(b, i);

		if (!_sbn_push_digit(ret, _sbn_add_digits(adig, bdig, &carry)))
			return sbn_free(ret), NULL;
	}

	/*
	 * Add `a`'s remaining digits, if any; Since `a` >= `b` in the number
	 * of digits, if `a` is still iterating, then we still have digits to
	 * add
	 */
	for (size_t i = minndigs; i < maxndigs; i++) {
		sbn_digit adig = sbn_nth_digit(a, i);
		if (!_sbn_push_digit(ret, _sbn_add_digits(adig, 0, &carry)))
			return sbn_free(ret), NULL;
	}

	/*
	 * Finally, add the remaining carry
	 */
	if (carry != 0 && !_sbn_push_digit(ret, carry))
		return sbn_free(ret), NULL;

	return ret;
}

/**
 * @brief Clone @a a
 */
struct sbn * sbn_clone (const struct sbn * a)
{
	struct sbn * ret = (a) ? sbn_new() : NULL;
	if (ret) {
		if (!_sbn_append_digits(ret, a))
			return sbn_free(ret), NULL;
		ret->is_negative = a->is_negative;
	}
	return ret;
}

/**
 * @brief Create a new SBN
 */
struct sbn * sbn_new (void)
{
	return calloc(1, sizeof(struct sbn));
}

/**
 * @brief Subtract @a b from @a a, ignoring the sign
 *
 * TODO: Subtract numbers of different signs
 *
 *  a -  b = a - b
 *  a - -b = a + b
 * -a -  b = -(a + b)
 * -a - -b = b - a
 */
struct sbn * sbn_sub_u (const struct sbn * a, const struct sbn * b)
{
	if (!a && !b)
		return NULL;

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);
	size_t minndigs = _sbn_min(andigs, bndigs);
	size_t maxndigs = _sbn_max(andigs, bndigs);

	/* if `b` is 0 then there's nothing to do */
	if (bndigs == 0)
		return sbn_clone(a);

	/* if `a` is 0 then we have to copy `b` to `a` */
	if (andigs == 0)
		return sbn_clone(b);

	struct sbn * ret = sbn_new();
	if (!ret) return NULL;
	if (!_sbn_reserve(ret, maxndigs)) return sbn_free(ret), NULL;

	sbn_digit carry = 0;

	for (size_t i = 0; i < minndigs; i++) {
		if (!_sbn_push_digit(ret, _sbn_sub_digits(
						sbn_nth_digit(a, i),
						sbn_nth_digit(b, i),
						&carry)))
			return sbn_free(ret), NULL;
	}

	/*
	 * If `a` and `b` have a different number of digits, then one of them
	 * is still iterating
	 */
	if (andigs < bndigs)
		a = b;

	for (size_t i = minndigs; i < maxndigs; i++)
		if (!_sbn_push_digit(ret, _sbn_sub_digits(
						sbn_nth_digit(a, i),
						0,
						&carry)))
			return sbn_free(ret), NULL;

	/*
	 * Finally, add the remaining carry
	 */
	if (carry != 0 && !_sbn_push_digit(ret, carry))
		return sbn_free(ret), NULL;

	return ret;
}

#endif /* SBN_CFG_IMPLEMENTATION */