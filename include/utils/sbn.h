/* sbn - v2023.03.13-1
 *
 * A bignum type inspired by
 *  * Scheme
 *  * [stb](https://github.com/nothings/stb)
 *  * [sort](https://github.com/swenson/sort)
 *
 * The most up to date version of this file can be found at
 * `include/utils/sbn.h` on [siiky/c-utils](https://git.sr.ht/~siiky/c-utils).
 *
 * It depends on `vec.h` which can be found next to this file in the link above.
 *
 * It can be configured to use different integer types for the digits, but
 * there can be only one implementation.
 *
 * _Modern Computer Arithmetic_ is used as a reference; you can get it here:
 * https://members.loria.fr/PZimmermann/mca/pub226.html
 */

# ifdef SBN_CFG_NO_STDINT

#  ifndef sbn_digit
#   error "Must define sbn_digit when using SBN_CFG_NO_STDINT"
#  endif /* sbn_digit */

#  ifndef sbn_double_digit
#   error "Must define sbn_double_digit when using SBN_CFG_NO_STDINT"
#  endif /* sbn_double_digit */

# else /* SBN_CFG_NO_STDINT */

/*
 * <stdint.h>
 *  uint32_t
 *  uint64_t
 */
#  include <stdint.h>

#  define sbn_digit              uint32_t
#  define sbn_double_digit       uint64_t

# endif /* SBN_CFG_NO_STDINT */

/*
 * <stdbool.h>
 *  bool
 *  false
 *  true
 *
 *  <stddef.h>
 *   size_t
 *
 *  <string.h>
 *   memcpy()
 *   memmove()
 *   strlen()
 */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* I don't like it as an opaque structure, but I can't think of a workaround */
struct sbn;

bool         sbn_add_digit_ud (struct sbn * a, const sbn_digit dig);
bool         sbn_eq           (const struct sbn * a, const struct sbn * b);
bool         sbn_ge           (const struct sbn * a, const struct sbn * b);
bool         sbn_gt           (const struct sbn * a, const struct sbn * b);
bool         sbn_is_negative  (const struct sbn * a);
bool         sbn_is_zero      (const struct sbn * a);
bool         sbn_le           (const struct sbn * a, const struct sbn * b);
bool         sbn_lt           (const struct sbn * a, const struct sbn * b);
bool         sbn_negate       (struct sbn * a);
bool         sbn_neq          (const struct sbn * a, const struct sbn * b);
bool         sbn_set_sign     (struct sbn * a, bool is_negative);
char *       sbn_to_str       (const struct sbn * a, unsigned base);
char *       sbn_to_str_16    (const struct sbn * a);
int          sbn_cmp          (const struct sbn * a, const struct sbn * b);
int          sbn_sign         (const struct sbn * a);
sbn_digit    sbn_nth_digit    (const struct sbn * a, size_t nth);
size_t       sbn_ndigits      (const struct sbn * a);
struct sbn * sbn_add          (const struct sbn * a, const struct sbn * b);
struct sbn * sbn_add_digit_u  (const struct sbn * a, const sbn_digit dig);
struct sbn * sbn_add_u        (const struct sbn * a, const struct sbn * b);
struct sbn * sbn_clone        (const struct sbn * a);
struct sbn * sbn_free         (struct sbn * a);
struct sbn * sbn_from_str     (size_t nchars, const char str[nchars], unsigned base);
struct sbn * sbn_from_str_16  (size_t nchars, const char str[nchars]);
struct sbn * sbn_new          (void);
struct sbn * sbn_sub_u        (const struct sbn * a, const struct sbn * b);

#ifdef SBN_CFG_IMPLEMENTATION

#define VEC_CFG_COPIABLE_DATA_TYPE
#define VEC_CFG_DATA_TYPE sbn_digit
#define VEC_CFG_IMPLEMENTATION
#define VEC_CFG_VEC _sbn_digits_vec
#include <utils/vec.h>

# ifdef SBN_CFG_NO_STDINT

#  ifndef sbn_digit_nbits
#   error "Must define sbn_digit_nbits when using SBN_CFG_NO_STDINT"
#  endif /* sbn_digit_nbits */

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

#  define sbn_digit_nbits        32U
#  define sbn_double_digit_nbits 64U

# endif /* SBN_CFG_NO_STDINT */

# define sbn_digit_nquartets              (sbn_digit_nbits >> 2)
# define sbn_digit_nbytes                 (sbn_digit_nbits >> 3)
# define sbn_nbits_diff                   (sbn_double_digit_nbits - sbn_digit_nbits)
# define sbn_double_digit_upper_half(dig) ((sbn_digit) ((dig) >> sbn_nbits_diff))
# define sbn_double_digit_lower_half(dig) sbn_double_digit_upper_half((dig) << sbn_nbits_diff)
# define sbn_twos_compl(dig)              (~(dig) + 1)
# define _sbn_min(a, b)                   (((a) < (b)) ? (a) : (b))
# define _sbn_max(a, b)                   (((a) > (b)) ? (a) : (b))

struct sbn {
	/** Is this SBN negative? */
	unsigned char is_negative : 1;

	/** The vector of digits */
	struct _sbn_digits_vec digits[1];
};

/*************************
 *************************
 *** Private Functions ***
 *************************
 *************************/

/*******************************
 * Memory Management Functions *
 *******************************/

/**
 * @brief Reserve a total number of digits
 */
static bool _sbn_reserve (struct sbn * a, size_t total_digs)
{ return a && _sbn_digits_vec_reserve(a->digits, total_digs); }

/**
 * @brief Append the digits of @a b to the end of @a a's digits
 */
static bool _sbn_append_digits (struct sbn * restrict a, const struct sbn * restrict b)
{ return a && b && _sbn_digits_vec_append(a->digits, b->digits); }

/**
 * @brief Append a new digit
 */
static bool _sbn_push_digit (struct sbn * a, sbn_digit dig)
{ return a && _sbn_digits_vec_push(a->digits, dig); }

static bool _sbn_flush_digits (struct sbn * a)
{
	return a && _sbn_digits_vec_set_len(a->digits, 0)
		&& _sbn_digits_vec_shrink_to_fit(a->digits);
}

/******************
 * Misc Functions *
 ******************/

#if 0
/**
 * @brief Reverse a string
 */
static bool _sbn_str_reverse (size_t nchars, char str[nchars])
{
	if (!str) return false;

	size_t mid = nchars >> 1;
	for (size_t i = 0; i < mid; i++) {
		char tmp = str[i];
		str[i] = str[nchars - i - 1];
		str[nchars - i - 1] = tmp;
	}

	return true;
}

/**
 * @brief Find the index of the first occurrence of a char that's not @a c
 */
static size_t _sbn_strnchr_idx (size_t nchars, char str[nchars], char c)
{
	if (!str) return 0;
	size_t ret = 0;
	for (ret = 0; ret < nchars && str[ret] == c; ret++);
	return ret;
}

/**
 * @brief Strip occurrences of @a c to the left of @a str
 */
static size_t _sbn_str_strip_left_chr (size_t nchars, char str[nchars], char c)
{
	if (!str) return 0;

	size_t nni = _sbn_strnchr_idx(nchars, str, c);
	if (nni == nchars + 1) return nchars; /* No occurrence, nothing to change */

	size_t newlen = nchars - nni;
	memmove(str, str + nni, nchars - nni);
	str[newlen] = '\0';
	return newlen;
}
#endif

/**
 * @brief Calculate a digit from a string
 */
static bool _sbn_digit_from_str_16 (size_t nchars, const char str[nchars], sbn_digit * dig, size_t * processed)
{
	if (!str || !dig || !processed) return false;

	size_t maxi = _sbn_min(nchars, sbn_digit_nquartets);
	sbn_digit ret = 0;

	for (size_t i = 0; i < maxi && str[i] != '\0'; i++) {
		/*
		 * The shift is at the top of the loop to avoid shifting on the
		 * last iteration; Since ret=0 in the first iteration, it
		 * doesn't change value
		 */
		ret <<= 4;

		char c = str[i];
		sbn_digit tmp = (c >= '0' && c <= '9') ?
			((sbn_digit) (c - '0')):
			(c >= 'a' && c <= 'f') ?
			((sbn_digit) (0xa + c - 'a')):
			42;
		if (tmp > 15) return false;
		ret |= tmp;
	}

	*processed = maxi;
	*dig = ret;
	return true;
}

/**
 * @brief Calculate the number significant quartets, i.e., the number of
 *        quartets in a digit, excluding zeroes to the left
 */
static size_t _sbn_digit_left_non_0_quartets (sbn_digit dig)
{
	const sbn_digit mod16 = 0xf;
	size_t ret = 0;
	for (; ret < sbn_digit_nquartets; ret++) {
		size_t shift = (sbn_digit_nquartets - ret - 1) << 2;
		size_t quart = (dig >> shift) & mod16;
		if (quart != 0)
			break;
	}
	return sbn_digit_nquartets - ret;
}

/**
 * @brief Convert a digit to a string in base 16
 */
static void _sbn_digit_to_str_16 (size_t nchars, char str[nchars], sbn_digit dig, size_t dig_nquarts)
{
	static const char map[16] = "0123456789abcdef";
	const sbn_digit mod16 = 0xf;

	size_t nquarts = (dig_nquarts) ? dig_nquarts : sbn_digit_nquartets;

	for (size_t qi = 0; qi < nquarts; qi++) {
		const size_t shift = qi << 2;
		const unsigned char c = (dig >> shift) & mod16;

		str[nquarts - qi - 1] = (c <= 0xf) ?
			map[c]:
			'!'; /* Shouldn't happen */
	}
}

static char * _sbn_not_0_to_str_16 (const struct sbn * a)
{
	const size_t ndigs = sbn_ndigits(a);
	const sbn_digit last_dig = sbn_nth_digit(a, ndigs - 1);
	const size_t last_dig_nquarts = _sbn_digit_left_non_0_quartets(last_dig);
	const size_t nchars = (ndigs - 1) * sbn_digit_nquartets + last_dig_nquarts;

	char * ret = calloc(nchars + 1, sizeof(char));
	if (!ret) return NULL;

	_sbn_digit_to_str_16(last_dig_nquarts, ret, last_dig, last_dig_nquarts);
	for (size_t di = 0; di < ndigs - 1; di--)
		_sbn_digit_to_str_16(sbn_digit_nquartets, ret + nchars - ((di + 1) * sbn_digit_nquartets),
				sbn_nth_digit(a, di),
				0);

	return ret;
}

static char * _sbn_0_to_str (void)
{
	char * ret = malloc(2 * sizeof(char));
	if (ret) {
		ret[0] = '0';
		ret[1] = '\0';
	}
	return ret;
}

static char * _sbn_any_to_str (char * (*not_0_to_str) (const struct sbn *), const struct sbn * a)
{
	return (!a) ?
		NULL:
		(sbn_is_zero(a)) ?
		_sbn_0_to_str():
		not_0_to_str(a);
}

/**
 * @brief Set the @a nth digit to @a dig
 */
static bool _sbn_set_nth_digit (struct sbn * a, size_t i, sbn_digit dig)
{ return a && _sbn_digits_vec_set_nth(a->digits, i, dig); }

/************************
 * Arithmetic Functions *
 ************************/

/**
 * @brief Calculate the result and carry of adding two digits
 */
static sbn_digit _sbn_add_digits (sbn_digit _a, sbn_digit _b, sbn_digit * _carry)
{
	sbn_double_digit carry = *_carry;
	sbn_double_digit a = _a;
	sbn_double_digit b = _b + carry;

	sbn_double_digit tmp = a + b;
	*_carry = sbn_double_digit_upper_half(tmp);
	return sbn_double_digit_lower_half(tmp);
}

/**
 * @brief Calculate the result and carry of subtracting two digits
 */
static sbn_digit _sbn_sub_digits (sbn_digit _a, sbn_digit _b, sbn_digit * _carry)
{
	sbn_double_digit carry = *_carry;
	sbn_double_digit a = _a;
	sbn_double_digit b = _b + carry;

	if (a >= b) {
		*_carry = 0;
		return a - b;
	} else {
		*_carry = 1;
		return b - a;
	}
}

static sbn_digit _sbn_mul_digits (sbn_digit _a, sbn_digit _b, sbn_digit * _carry)
{
	sbn_double_digit a = _a;
	sbn_double_digit b = _b;
	sbn_double_digit carry = *_carry;

	sbn_double_digit tmp = a * b + carry;
	*_carry = sbn_double_digit_upper_half(tmp);
	return sbn_double_digit_lower_half(tmp);
}

/************************
 ************************
 *** Public Functions ***
 ************************
 ************************/

/*******************************
 * Memory Management Functions *
 *******************************/

/**
 * @brief Create a new SBN
 */
struct sbn * sbn_new (void)
{ return calloc(1, sizeof(struct sbn)); }

/**
 * @brief Free @a a
 */
struct sbn * sbn_free (struct sbn * a)
{
	if (a) {
		*a->digits = _sbn_digits_vec_free(*a->digits);
		a->is_negative = 0;
		free(a);
	}
	return NULL;
}

/**
 * @brief Clone @a a
 */
struct sbn * sbn_clone (const struct sbn * a)
{
	struct sbn * ret = (a) ? sbn_new() : NULL;
	if (ret) {
		bool succ = _sbn_append_digits(ret, a);
		if (succ)
			ret->is_negative = a->is_negative;
		else
			ret = sbn_free(ret);
	}
	return ret;
}

/************************
 * Comparison Functions *
 ************************/

/**
 * @brief Is @a a zero?
 *
 * TODO: Is an SBN 0 only when it has no digits?
 */
bool sbn_is_zero (const struct sbn * a)
{ return sbn_ndigits(a) == 0; }

/**
 * @brief Compare @a a and @a b a la `strcmp()`
 */
int sbn_cmp (const struct sbn * a, const struct sbn * b)
{
	if (a == b) return 0;
	if (!a) return -1;
	if (!b) return 1;

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);

	if (andigs < bndigs) return -1;
	if (andigs > bndigs) return 1;

	int ret = 0;
	for (size_t i = 0; i < andigs && ret == 0; i++) {
		size_t nth = andigs - i - 1;
		sbn_digit adig = sbn_nth_digit(a, nth);
		sbn_digit bdig = sbn_nth_digit(b, nth);
		ret = (adig < bdig) ?
			-1:
			(adig > bdig) ?
			1:
			0;
	}
	return ret;
}

/**
 * @brief @a a == @a b
 */
bool sbn_eq (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp(a, b) == 0; }

/**
 * @brief @a a != @a b
 */
bool sbn_neq (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp(a, b) != 0; }

/**
 * @brief @a a > @a b
 */
bool sbn_gt (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp(a, b) > 0; }

/**
 * @brief @a a < @a b
 */
bool sbn_lt (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp(a, b) < 0; }

/**
 * @brief @a a >= @a b
 */
bool sbn_ge (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp(a, b) >= 0; }

/**
 * @brief @a a <= @a b
 */
bool sbn_le (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp(a, b) <= 0; }

/******************
 * Misc Functions *
 ******************/

/**
 * @brief Is @a a negative?
 */
bool sbn_is_negative (const struct sbn * a)
{ return a && a->is_negative; }

/**
 * @brief Negate @a a
 */
bool sbn_negate (struct sbn * a)
{return sbn_set_sign(a, !sbn_is_negative(a)); }

/**
 * @brief Set @a a's sign
 */
bool sbn_set_sign (struct sbn * a, bool is_negative)
{ return a && ((a->is_negative = is_negative), true); }

/**
 * @brief Get the sign of @a a
 */
int sbn_sign (const struct sbn * a)
{ return (sbn_is_zero(a)) ? 0: (sbn_is_negative(a)) ? -1: 1; }

/**
 * @brief Get the @a nth digit of @a a
 */
sbn_digit sbn_nth_digit (const struct sbn * a, size_t nth)
{ return _sbn_digits_vec_get_nth(a->digits, nth); }

/**
 * @brief Get the number of digits of @a a
 */
size_t sbn_ndigits (const struct sbn * a)
{ return (a) ? _sbn_digits_vec_len(a->digits) : 0; }

/**
 * @brief Convert an SBN to a string in base 16
 */
char * sbn_to_str_16 (const struct sbn * a)
{ return _sbn_any_to_str(_sbn_not_0_to_str_16, a); }

/**
 * @brief Convert an SBN to a string
 */
char * sbn_to_str (const struct sbn * a, unsigned base)
{
	/* TODO: Convert to bases other than 16 */
	return (!a) ?
		NULL:
		(base == 16) ?
		sbn_to_str_16(a):
		NULL;
}

/**
 * @brief Convert a string to an SBN in base 16
 */
struct sbn * sbn_from_str_16 (size_t nchars, const char str[nchars])
{
	if (!str) return NULL;

	nchars = (nchars == 0) ? strlen(str) : nchars;
	const size_t nwhole_digs = nchars / sbn_digit_nquartets;
	const size_t last_dig = nchars % sbn_digit_nquartets;
	const size_t ndigs = nwhole_digs + last_dig;

	struct sbn * ret = sbn_new();
	if (!ret || !_sbn_reserve(ret, ndigs))
		return sbn_free(ret);

	while (nchars > 0) {
		sbn_digit dig = 0;
		size_t processed = _sbn_min(sbn_digit_nquartets, nchars);
		if (!_sbn_digit_from_str_16(processed, str + nchars - processed, &dig, &processed)
				|| !_sbn_push_digit(ret, dig))
			return sbn_free(ret);
		nchars -= processed;
	}

	return ret;
}

/**
 * @brief Convert a string to an SBN
 */
struct sbn * sbn_from_str (size_t nchars, const char str[nchars], unsigned base)
{
	/* TODO: Convert to bases other than 16 */
	return (!str) ?
		NULL:
		(base == 16) ?
		sbn_from_str_16(nchars, str):
		NULL;
}

/************************
 * Arithmetic Functions *
 ************************/

/**
 * TODO: Optimise this
 */
bool sbn_digit_left_shift_d (struct sbn * a, size_t shift)
{
	if (!a) return false;
	bool ret = true;
	for (size_t i = 0; ret && i < shift; i++)
		ret = _sbn_digits_vec_insert(a->digits, 0, 0);
	return ret;
}

/**
 * TODO: Optimise this
 */
bool sbn_digit_right_shift_d (struct sbn * a, size_t shift)
{
	if (!a) return false;
	bool ret = true;
	for (size_t i = 0; ret && i < shift; i++)
		ret = _sbn_digits_vec_remove(a->digits, 0);
	return ret;
}

struct sbn * sbn_digit_left_shift (const struct sbn * a, size_t shift)
{
	struct sbn * ret = sbn_clone(a);
	return (sbn_digit_left_shift_d(ret, shift)) ? ret : sbn_free(ret);
}

struct sbn * sbn_digit_right_shift (const struct sbn * a, size_t shift)
{
	struct sbn * ret = sbn_clone(a);
	return (sbn_digit_right_shift_d(ret, shift)) ? ret : sbn_free(ret);
}

/**
 * @brief Add a single digit to @a a, destructively, ignoring the sign
 */
bool sbn_add_digit_ud (struct sbn * a, const sbn_digit dig)
{
	if (!a) return false;
	if (dig == 0) return true;

	sbn_digit carry = dig;
	size_t ndigs = sbn_ndigits(a);
	bool ret = true;

	for (size_t i = 0; ret && carry && i < ndigs; i++)
		ret = _sbn_set_nth_digit(a, i, _sbn_add_digits(sbn_nth_digit(a, i), 0, &carry));

	return ret && (!carry || _sbn_push_digit(a, carry));
}

/**
 * @brief Add a single digit to @a a, non-destructively, ignoring the sign
 */
struct sbn * sbn_add_digit_u (const struct sbn * a, const sbn_digit dig)
{
	struct sbn * ret = sbn_clone(a);
	return (ret && !sbn_add_digit_ud(ret, dig)) ?
		sbn_free(ret):
		ret;
}

/**
 * @brief Add @a b to @a a, ignoring the sign
 */
struct sbn * sbn_add_u (const struct sbn * a, const struct sbn * b)
{
	if (!a && !b)
		return NULL;

	/* if `a` is 0 then we have to copy `b` to `a` */
	if (sbn_is_zero(a)) return sbn_clone(b);

	/* if `b` is 0 then there's nothing to do */
	if (sbn_is_zero(b)) return sbn_clone(a);

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);

	/*
	 * Since we're going to clone one of `a` and `b`, might as well clone
	 * the one with more digits; from this `if` forward, `a` >= `b` in the
	 * number of digits
	 */
	if (andigs < bndigs) {
		{ const struct sbn * tmp = a; a = b; b = tmp; }
		{ size_t tmp = andigs; andigs = bndigs; bndigs = tmp; }
	}

	struct sbn * ret = sbn_new();
	if (!ret) return NULL;
	if (!_sbn_reserve(ret, andigs)) return sbn_free(ret);

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
			return sbn_free(ret);
	}

	/*
	 * Add `a`'s remaining digits, if any; Since `a` >= `b` in the number
	 * of digits, if `a` is still iterating, then we still have digits to
	 * add
	 */
	for (size_t i = minndigs; i < maxndigs; i++) {
		sbn_digit adig = sbn_nth_digit(a, i);
		if (!_sbn_push_digit(ret, _sbn_add_digits(adig, 0, &carry)))
			return sbn_free(ret);
	}

	/*
	 * Finally, add the remaining carry
	 */
	if (carry && !_sbn_push_digit(ret, carry))
		return sbn_free(ret);

	return ret;
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
 * @brief Subtract @a b from @a a, ignoring the sign
 */
struct sbn * sbn_sub_u (const struct sbn * a, const struct sbn * b)
{
	size_t bndigs = sbn_ndigits(b);
	/* if `b` is 0 then there's nothing to do */
	if (bndigs == 0) return sbn_clone(a);

	size_t andigs = sbn_ndigits(a);
	/* if `a` is 0 then we have to copy `b` and negate it (TODO) */
	if (andigs == 0) return sbn_clone(b);

	size_t maxndigs = _sbn_max(andigs, bndigs);
	struct sbn * ret = sbn_new();
	if (!ret || !_sbn_reserve(ret, maxndigs)) return sbn_free(ret);

	size_t minndigs = _sbn_min(andigs, bndigs);
	sbn_digit carry = 0;
	for (size_t i = 0; i < minndigs; i++) {
		if (!_sbn_push_digit(ret, _sbn_sub_digits(
						sbn_nth_digit(a, i),
						sbn_nth_digit(b, i),
						&carry)))
			return sbn_free(ret);
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
			return sbn_free(ret);

	/*
	 * Finally, add the remaining carry
	 */
	if (carry && !_sbn_push_digit(ret, carry))
		return sbn_free(ret);

	return ret;
}

/**
 * @brief Subtract @a b from @a a
 *
 * TODO: Subtract numbers of different signs
 *
 * Same sign:
 *  a -  b = a - b
 * -a - -b = b - a
 *
 * Different sign:
 *  a - -b = a + b
 * -a -  b = -(a + b)
 */
struct sbn * sbn_sub (const struct sbn * a, const struct sbn * b)
{
	struct sbn * ret = NULL;
	bool is_a_negative = sbn_is_negative(a);

	if (is_a_negative != sbn_is_negative(b)) {
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

bool sbn_mul_digit_ud (struct sbn * a, const sbn_digit dig)
{
	if (!a) return false;
	if (sbn_is_zero(a)) return true;
	if (dig == 0) return _sbn_flush_digits(a);

	size_t ndigs = sbn_ndigits(a);
	sbn_digit carry = 0;
	bool ret = _sbn_set_nth_digit(a, 0, _sbn_mul_digits(sbn_nth_digit(a, 0), dig, &carry));
	for (size_t i = 1; ret && i < ndigs && carry; i++)
		ret = _sbn_set_nth_digit(a, i, _sbn_mul_digits(sbn_nth_digit(a, i), 1, &carry));

	return ret && (!carry || _sbn_push_digit(a, carry));
}

struct sbn * sbn_mul_digit_u (const struct sbn * a, const sbn_digit dig)
{
	if (!a) return NULL;
	if (dig == 0) return sbn_new();
	struct sbn * ret = sbn_clone(a);
	return (ret && !sbn_mul_digit_ud(ret, dig)) ?
		sbn_free(ret):
		ret;
}

struct sbn * sbn_mul_u (const struct sbn * a, const struct sbn * b)
{
	if (!a || !b) return NULL;

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);

	if (sbn_is_zero(a) || sbn_is_zero(b)) return sbn_new();

	if (andigs < bndigs) {
		{ const struct sbn * tmp = a; a = b; b = tmp; }
		{ size_t tmp = andigs; andigs = bndigs; bndigs = tmp; }
	}

	struct sbn * ret = sbn_clone(a);
	if (!ret) return NULL;

	return ret;
}

#endif /* SBN_CFG_IMPLEMENTATION */
