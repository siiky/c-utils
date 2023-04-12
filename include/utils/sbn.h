/* sbn - v2023.04.12-6
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
 * https://maths-people.anu.edu.au/~brent/pd/mca-cup-0.5.9.pdf
 * https://members.loria.fr/PZimmermann/mca/pub226.html
 */

# ifndef sbn_digit
/*
 * <stdint.h>
 *  uint64_t
 */
#  include <stdint.h>
#  define sbn_digit uint64_t
# endif /* sbn_digit */

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

#define VEC_CFG_COPIABLE_DATA_TYPE
#define VEC_CFG_DATA_TYPE sbn_digit
#define VEC_CFG_VEC _sbn_digits_vec
#include <utils/vec.h>

/*
 * TODO: optimize space usage -- `digits` uses 40b while `is_negative` uses 8b
 * (because of alignment?). Won't be a problem once the `digits` vector is
 * "inlined".
 */
struct sbn {
	/** Is this SBN negative? */
	unsigned char is_negative : 1;

	/** The vector of digits */
	struct _sbn_digits_vec digits[1];
};

bool         sbn_add          (struct sbn * r, const struct sbn * a, const struct sbn * b);
bool         sbn_add_d        (struct sbn * a, const struct sbn * b);
bool         sbn_add_digit    (struct sbn * r, const struct sbn * a, const sbn_digit d);
bool         sbn_add_digit_d  (struct sbn * a, const sbn_digit d);
bool         sbn_add_digit_u  (struct sbn * r, const struct sbn * a, const sbn_digit d);
bool         sbn_add_digit_ud (struct sbn * a, const sbn_digit d);
bool         sbn_add_u        (struct sbn * r, const struct sbn * a, const struct sbn * b);
bool         sbn_add_ud       (struct sbn * a, const struct sbn * b);
bool         sbn_clone_to     (struct sbn * d, const struct sbn * s);
bool         sbn_eq_u         (const struct sbn * a, const struct sbn * b);
bool         sbn_from_str     (struct sbn * r, size_t nchars, const char str[nchars], unsigned base);
bool         sbn_from_str_16  (struct sbn * r, size_t nchars, const char str[nchars]);
bool         sbn_ge_u         (const struct sbn * a, const struct sbn * b);
bool         sbn_gt_u         (const struct sbn * a, const struct sbn * b);
bool         sbn_is_negative  (const struct sbn * a);
bool         sbn_is_zero      (const struct sbn * a);
bool         sbn_le_u         (const struct sbn * a, const struct sbn * b);
bool         sbn_lt_u         (const struct sbn * a, const struct sbn * b);
bool         sbn_mul          (struct sbn * r, const struct sbn * a, const struct sbn * b);
bool         sbn_mul_digit_u  (struct sbn * r, const struct sbn * a, const sbn_digit d);
bool         sbn_mul_digit_ud (struct sbn * a, const sbn_digit d);
bool         sbn_mul_u        (struct sbn * r, const struct sbn * a, const struct sbn * b);
bool         sbn_neq_u        (const struct sbn * a, const struct sbn * b);
bool         sbn_shl          (struct sbn * r, const struct sbn * a, size_t n);
bool         sbn_shl_d        (struct sbn * a, size_t n);
bool         sbn_shr          (struct sbn * r, const struct sbn * a, size_t n);
bool         sbn_shr_d        (struct sbn * a, size_t n);
bool         sbn_sub          (struct sbn * r, const struct sbn * a, const struct sbn * b);
bool         sbn_sub_digit_ud (struct sbn * a, const sbn_digit d);
bool         sbn_sub_u        (struct sbn * r, const struct sbn * a, const struct sbn * b);
char *       sbn_to_str       (const struct sbn * a, unsigned base);
char *       sbn_to_str_16    (const struct sbn * a);
int          sbn_cmp_u        (const struct sbn * a, const struct sbn * b);
int          sbn_sign         (const struct sbn * a);
sbn_digit    sbn_nth_digit    (const struct sbn * a, size_t nth);
size_t       sbn_ndigits      (const struct sbn * a);
struct sbn * sbn_abs          (struct sbn * a);
struct sbn * sbn_negate       (struct sbn * a);
struct sbn * sbn_set_sign     (struct sbn * a, bool is_negative);

/*
 * For each OP of '(add sub mul div), these will be all the functions defined:
 * bool sbn_OP(struct sbn * r, const struct sbn * a, const struct sbn * b);
 * bool sbn_OP_d(struct sbn * a, const struct sbn * b);
 * bool sbn_OP_u(struct sbn * r, const struct sbn * a, const struct sbn * b);
 * bool sbn_OP_ud(struct sbn * a, const struct sbn * b);
 * bool sbn_OP_digit(struct sbn * r, const struct sbn * a, const sbn_digit d);
 * bool sbn_OP_digit_d(struct sbn * a, const sbn_digit d);
 * bool sbn_OP_digit_u(struct sbn * r, const struct sbn * a, const sbn_digit d);
 * bool sbn_OP_digit_ud(struct sbn * a, const sbn_digit d);
 */

#ifdef SBN_CFG_IMPLEMENTATION

#define VEC_CFG_NO_REDEFINE_VEC
#define VEC_CFG_COPIABLE_DATA_TYPE
#define VEC_CFG_DATA_TYPE sbn_digit
#define VEC_CFG_IMPLEMENTATION
#define VEC_CFG_VEC _sbn_digits_vec
#include <utils/vec.h>

# ifndef sbn_digit_nbits
/*
 * <limits.h>
 *  CHAR_BIT
 */
#  include <limits.h>
#  define sbn_digit_nbits (CHAR_BIT * sizeof(sbn_digit))
# endif /* sbn_digit_nbits */

# define sbn_digit_half_nbits          (sbn_digit_nbits >> 1)
# define sbn_digit_nquartets           (sbn_digit_nbits >> 2)
# define sbn_digit_nbytes              (sbn_digit_nbits >> 3)
# define sbn_digit_upper_half(dig)     ((sbn_digit) ((dig) >> sbn_digit_half_nbits))
# define sbn_digit_lower_half(dig)     ((sbn_digit) ((dig) & ((((sbn_digit) 0x1) << sbn_digit_half_nbits) - 1)))
# define sbn_digit_from_halves(hi, lo) ((((sbn_digit) (hi)) << sbn_digit_half_nbits) | ((sbn_digit) (lo)))
# define sbn_digit_twos_compl(dig)     (~(dig) + 1)
# define sbn_min(a, b)                 (((a) < (b)) ? (a) : (b))
# define sbn_max(a, b)                 (((a) > (b)) ? (a) : (b))

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
{ return a && _sbn_digits_vec_set_len(a->digits, 0); }

/******************
 * Misc Functions *
 ******************/

static bool _sbn_drop_left_zeros (struct sbn * a)
{
	size_t ndigs = sbn_ndigits(a);
	while (ndigs > 0 && sbn_nth_digit(a, ndigs-1) == 0) {
		_sbn_digits_vec_pop(a->digits);
		ndigs--;
	}
	return true;
}

/**
 * @brief Calculate a digit from a string
 */
static bool _sbn_digit_from_str_16 (size_t nchars, const char str[nchars], sbn_digit * dig)
{
	if (!str || !dig) return false;

	sbn_digit ret = 0;
	for (size_t i = 0; i < nchars && str[i] != '\0'; i++) {
		/*
		 * The shift is at the top of the loop to avoid shifting on the last
		 * iteration; Since ret=0 in the first iteration, it doesn't change
		 * value
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

	*dig = ret;
	return true;
}

/**
 * @brief Calculate the number significant quartets, i.e., the number of
 *        quartets in a digit, excluding zeros to the left
 */
static size_t _sbn_digit_significant_nquartets (sbn_digit dig)
{
	size_t ret = 0;
	for (; dig; dig >>= 4) ret++;
	return ret;
}

/**
 * @brief Convert a digit to a string in base 16
 */
static void _sbn_digit_to_str_16 (sbn_digit dig, size_t nquarts, char str[nquarts])
{
	static const char map[16] = "0123456789abcdef";
	const sbn_digit mod16 = 0xf;
	for (size_t qi = 1; qi <= nquarts; qi++) {
		str[nquarts - qi] = map[dig & mod16];
		dig >>= 4;
	}
}

/**
 * @brief Convert a non-zero sbn to its string representation in base 16.
 */
static char * _sbn_not_0_to_str_16 (const struct sbn * a)
{
	const size_t ndigs = sbn_ndigits(a);
	const sbn_digit last_dig = sbn_nth_digit(a, ndigs - 1);
	const size_t last_dig_nquarts = _sbn_digit_significant_nquartets(last_dig);
	const size_t minus = sbn_is_negative(a) ? 1 : 0;
	const size_t nchars = (ndigs - 1) * sbn_digit_nquartets + last_dig_nquarts + minus;

	char * ret = calloc(nchars + 1, sizeof(char));
	if (!ret) return NULL;

	*ret = '-'; /* The minus sign will be overridden if a isn't negative */

	char * str = ret + minus;
	_sbn_digit_to_str_16(last_dig, last_dig_nquarts, str);
	str += last_dig_nquarts;
	for (size_t di = ndigs-1; di > 0; di--) {
		sbn_digit adig = sbn_nth_digit(a, di-1);
		_sbn_digit_to_str_16(adig, sbn_digit_nquartets, str);
		str += sbn_digit_nquartets;
	}

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
static sbn_digit _sbn_digit_add (sbn_digit a, sbn_digit b, sbn_digit * carry)
{
	sbn_digit c = *carry;
	sbn_digit r = a; r += b; r += c;
	*carry = (c ? (r <= a) : (r < a)) ? 1 : 0;
	return r;
}

/**
 * @brief Calculate the result and borrow of subtracting two digits
 */
static sbn_digit _sbn_digit_sub (sbn_digit a, sbn_digit b, sbn_digit * borrow)
{
	sbn_digit c = *borrow;
	sbn_digit r = a; r -= b; r -= c;
	*borrow = (c ? (r >= a) : (r > a)) ? 1 : 0;
	return r;
}

/**
 * @brief Calculate the result and carry of multiplying two digits
 */
static sbn_digit _sbn_digit_mul (sbn_digit a, sbn_digit b, sbn_digit * carry)
{
	sbn_digit c = *carry;

	sbn_digit al = sbn_digit_lower_half(a);
	sbn_digit ah = sbn_digit_upper_half(a);
	sbn_digit bl = sbn_digit_lower_half(b);
	sbn_digit bh = sbn_digit_upper_half(b);

	sbn_digit p1 = ah * bl;
	sbn_digit p2 = al * bh;

	sbn_digit r = al * bl;
	r = _sbn_digit_add(r, sbn_digit_lower_half(p1) << sbn_digit_half_nbits, &c);
	r = _sbn_digit_add(r, sbn_digit_lower_half(p2) << sbn_digit_half_nbits, &c);

	sbn_digit cr = ah * bh;
	cr = _sbn_digit_add(cr, sbn_digit_upper_half(p1), &c);
	cr = _sbn_digit_add(cr, sbn_digit_upper_half(p2), &c);
	assert(!c);

	*carry = cr;
	return r;
}

/************************
 ************************
 *** Public Functions ***
 ************************
 ************************/

/*******************************
 * Memory Management Functions *
 *******************************/

struct sbn * sbn_destroy (struct sbn * a)
{
	if (a) {
		*a->digits = _sbn_digits_vec_free(*a->digits);
		a->is_negative = 0;
	}
	return a;
}

bool sbn_clone_to (struct sbn * d, const struct sbn * s)
{
	if (d && s && _sbn_flush_digits(d) && _sbn_append_digits(d, s)) {
		d->is_negative = s->is_negative;
		return true;
	}
	return false;
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
{
	size_t ndigs = sbn_ndigits(a);
	if (ndigs == 0) return true;
	bool all_zero = true;
	for (size_t i = ndigs; all_zero && i > 0; i--)
		all_zero = sbn_nth_digit(a, i-1) == 0;
	return all_zero;
}

/**
 * @brief Compare @a a and @a b a la `strcmp()`
 */
int sbn_cmp_u (const struct sbn * a, const struct sbn * b)
{
	if (a == b) return 0;
	if (!a) return -1;
	if (!b) return 1;

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);
	int ret = (andigs < bndigs) ? -1 : (andigs > bndigs) ? 1 : 0;
	for (size_t i = 0; !ret && i < andigs; i++) {
		size_t nth = andigs - i - 1;
		sbn_digit adig = sbn_nth_digit(a, nth);
		sbn_digit bdig = sbn_nth_digit(b, nth);
		ret = (adig < bdig) ? -1 : (adig > bdig) ? 1 : 0;
	}
	return ret;
}

/**
 * @brief @a a == @a b
 */
bool sbn_eq_u (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp_u(a, b) == 0; }

/**
 * @brief @a a != @a b
 */
bool sbn_neq_u (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp_u(a, b) != 0; }

/**
 * @brief @a a > @a b
 */
bool sbn_gt_u (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp_u(a, b) > 0; }

/**
 * @brief @a a < @a b
 */
bool sbn_lt_u (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp_u(a, b) < 0; }

/**
 * @brief @a a >= @a b
 */
bool sbn_ge_u (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp_u(a, b) >= 0; }

/**
 * @brief @a a <= @a b
 */
bool sbn_le_u (const struct sbn * a, const struct sbn * b)
{ return sbn_cmp_u(a, b) <= 0; }

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
struct sbn * sbn_negate (struct sbn * a)
{
	if (a) a->is_negative = !a->is_negative;
	return a;
}

/**
 * @brief Set @a a's sign
 */
struct sbn * sbn_set_sign (struct sbn * a, bool is_negative)
{
	if (a) a->is_negative = is_negative;
	return a;
}

/**
 * @brief Get the sign of @a a
 */
int sbn_sign (const struct sbn * a)
{ return sbn_is_zero(a) ? 0: sbn_is_negative(a) ? -1: 1; }

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
bool sbn_from_str_16 (struct sbn * r, size_t nchars, const char str[nchars])
{
	if (!r || !str || nchars == 0) return false;

	if (str[0] == '-') {
		sbn_set_sign(r, true);
		nchars--;
		str++;
	}

	while (nchars > 0 && str[0] == '0') str++, nchars--;
	if (nchars == 0) return _sbn_flush_digits(r);

	const size_t nwhole_digs = nchars / sbn_digit_nquartets;
	const size_t last_dig_nquarts = nchars % sbn_digit_nquartets;
	const size_t ndigs = nwhole_digs + (last_dig_nquarts ? 1 : 0);
	if (!_sbn_reserve(r, ndigs)) return false;

	bool ret = true;
	while (ret && nchars > 0) {
		sbn_digit dig = 0;
		size_t processed = sbn_min(sbn_digit_nquartets, nchars);
		nchars -= processed;
		ret = _sbn_digit_from_str_16(processed, str + nchars, &dig)
			&& _sbn_push_digit(r, dig);
	}

	return ret;
}

/**
 * @brief Convert a string to an SBN
 */
bool sbn_from_str (struct sbn * r, size_t nchars, const char str[nchars], unsigned base)
{
	/* TODO: Convert to bases other than 16 */
	return (base == 16) ? sbn_from_str_16(r, nchars, str) : false;
}

/************************
 * Arithmetic Functions *
 ************************/

struct sbn * sbn_abs (struct sbn * a)
{ return sbn_set_sign(a, false); }

/**
 * @brief Add a single digit to @a a, destructively, ignoring the sign
 */
bool sbn_add_digit_ud (struct sbn * a, const sbn_digit dig)
{
	if (!a) return false;
	if (dig == 0) return true;
	if (sbn_is_zero(a)) return _sbn_push_digit(a, dig);

	size_t ndigs = sbn_ndigits(a);
	sbn_digit carry = dig;
	bool ret = true;
	for (size_t i = 0; ret && carry && i < ndigs; i++)
		ret = _sbn_set_nth_digit(a, i, _sbn_digit_add(sbn_nth_digit(a, i), 0, &carry));
	return ret && (!carry || _sbn_push_digit(a, 1));
}

/**
 * @brief Add @a b to @a a, ignoring the sign
 */
bool sbn_add_u (struct sbn * r, const struct sbn * a, const struct sbn * b)
{
	if (!_sbn_flush_digits(r)) return false;

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);
	if (!andigs && !bndigs) return _sbn_flush_digits(r), true;
	if (!andigs) return sbn_clone_to(r, b);
	if (!bndigs) return sbn_clone_to(r, a);

	/*
	 * Since we're going to clone one of `a` and `b`, might as well clone the
	 * one with more digits; from this `if` forward, `a` >= `b` in the number
	 * of digits
	 */
	if (andigs < bndigs) {
		{ const struct sbn * tmp = a; a = b; b = tmp; }
		{ size_t tmp = andigs; andigs = bndigs; bndigs = tmp; }
	}

	if (!_sbn_reserve(r, andigs+1)) return false;

	size_t minndigs = bndigs;
	size_t maxndigs = andigs;
	sbn_digit carry = 0;
	bool ret = true;

	/* Add common digits */
	for (size_t i = 0; ret && i < minndigs; i++) {
		sbn_digit adig = sbn_nth_digit(a, i);
		sbn_digit bdig = sbn_nth_digit(b, i);
		ret = _sbn_push_digit(r, _sbn_digit_add(adig, bdig, &carry));
	}
	if (!ret) return _sbn_flush_digits(r), false;

	/*
	 * Add `a`'s remaining digits, if any; Since `a` >= `b` in the number of
	 * digits, if `a` is still iterating, then we still have digits to add
	 */
	for (size_t i = minndigs; ret && i < maxndigs; i++) {
		sbn_digit adig = sbn_nth_digit(a, i);
		ret = _sbn_push_digit(r, _sbn_digit_add(adig, 0, &carry));
	}
	if (!ret) return _sbn_flush_digits(r), false;

	/* Finally, add the remaining carry */
	return !carry || _sbn_push_digit(r, carry);
}

/**
 * @brief Add @a b to @a a
 */
bool sbn_add (struct sbn * r, const struct sbn * a, const struct sbn * b)
{
	if (sbn_is_negative(a)) {
		if (sbn_is_negative(b)) /* -a + -b = -(a + b) */
			return sbn_add_u(r, a, b)
				&& (sbn_set_sign(r, true), true);
		else /* -a + b = b - a */
			return sbn_sub_u(r, b, a)
				&& (sbn_set_sign(r, sbn_lt_u(b, a)), true);
	} else {
		if (sbn_is_negative(b)) /* a + -b = a - b */
			return sbn_sub_u(r, a, b)
				&& (sbn_set_sign(r, sbn_lt_u(a, b)), true);
		else /* a + b = a + b */
			return sbn_add_u(r, a, b)
				&& (sbn_set_sign(r, false), true);
	}
}

bool sbn_add_d (struct sbn * a, const struct sbn * b)
{
	struct sbn r[1];
	if (sbn_add(r, a, b)) {
		sbn_destroy(a);
		*a = *r;
		return true;
	}
	return false;
}

bool sbn_add_digit (struct sbn * r, const struct sbn * a, const sbn_digit d)
{
	return sbn_clone_to(r, a) && sbn_add_digit_d(r, d);
}

bool sbn_add_digit_d (struct sbn * a, const sbn_digit d)
{
	return sbn_is_negative(a) ? sbn_sub_digit_ud(a, d) : sbn_add_digit_ud(a, d);
}

/**
 * @brief Add a single digit to @a a, non-destructively, ignoring the sign
 */
bool sbn_add_digit_u (struct sbn * r, const struct sbn * a, const sbn_digit d)
{
	return sbn_clone_to(r, a) && sbn_add_digit_ud(r, d);
}

/* TODO: Optimize to modify a in-place */
bool sbn_add_ud (struct sbn * a, const struct sbn * b)
{
	struct sbn r[1];
	if (sbn_add_u(r, a, b)) {
		sbn_destroy(a);
		*a = *r;
		return true;
	}
	return false;
}

bool sbn_sub_digit_ud (struct sbn * a, const sbn_digit d)
{
	(void) a;
	(void) d;
	return false; /* TODO */
}

/**
 * @brief Subtract @a b from @a a, ignoring the sign
 *
 * @a r MUST NOT be @a a nor @a b!
 */
bool sbn_sub_u (struct sbn * r, const struct sbn * a, const struct sbn * b)
{
	if (!r || !_sbn_flush_digits(r)) return false;

	/* Let's make life easier for ourselves... */
	bool is_negative = sbn_lt_u(a, b);
	if (is_negative) {
		const struct sbn * tmp = a;
		a = b;
		b = tmp;
	}

	bool azero = sbn_is_zero(a);
	bool bzero = sbn_is_zero(b);
	if (azero && bzero) return true;
	if (azero) return sbn_clone_to(r, b) && sbn_set_sign(r, is_negative);
	if (bzero) return sbn_clone_to(r, a) && sbn_set_sign(r, is_negative);

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);
	size_t maxndigs = sbn_max(andigs, bndigs);
	if (!_sbn_reserve(r, maxndigs)) return false;

	bool succ = true;
	size_t minndigs = sbn_min(andigs, bndigs);
	sbn_digit borrow = 0;
	size_t i = 0;
	for (; succ && i < minndigs; i++) {
		sbn_digit adig = sbn_nth_digit(a, i);
		sbn_digit bdig = sbn_nth_digit(b, i);
		succ = _sbn_push_digit(r, _sbn_digit_sub(adig, bdig, &borrow));
	}

	/*
	 * If `a` and `b` have a different number of digits, `a` is still
	 * iterating, propagate borrow
	 */
	for (i = minndigs; succ && borrow && i < maxndigs; i++) {
		sbn_digit adig = sbn_nth_digit(a, i);
		succ = _sbn_push_digit(r, _sbn_digit_sub(adig, 0, &borrow));
	}

	for (; succ && i < maxndigs; i++)
		succ = _sbn_push_digit(r, sbn_nth_digit(a, i));

	if (!succ) return _sbn_flush_digits(r), false;

	sbn_set_sign(r, is_negative);
	return _sbn_drop_left_zeros(r);
}

/**
 * @brief Subtract @a b from @a a
 */
bool sbn_sub (struct sbn * r, const struct sbn * a, const struct sbn * b)
{
	if (sbn_is_negative(a)) {
		if (sbn_is_negative(b)) /* -a - -b = b - a */
			return sbn_sub_u(r, b, a);
		else /* -a - b = -(a + b) */
			return sbn_add_u(r, a, b) && (sbn_negate(r), true);
	} else {
		if (sbn_is_negative(b)) /* a - -b = a + b */
			return sbn_add_u(r, a, b);
		else /* a - b = a - b */
			return sbn_sub_u(r, a, b);
	}
}

/**
 * @brief Shift @a a @a n digits (not bits) to the left.
 */
bool sbn_shl_d (struct sbn * a, size_t n)
{
	if (!a) return false;
	size_t ndigs = sbn_ndigits(a);
	if (n == 0 || ndigs == 0) return true;
	bool ret = _sbn_digits_vec_reserve(a->digits, ndigs + n);
	/* TODO: Optimize this */
	/* if reserve succeeds inserts shouldn't fail -- just sanity check */
	for (; ret && n > 0; n--) ret = _sbn_digits_vec_insert(a->digits, 0, 0);
	return ret;
}

bool sbn_shl (struct sbn * r, const struct sbn * a, size_t n)
{ return sbn_clone_to(r, a) && sbn_shl_d(r, n); }

/**
 * @brief Shift @a a @a n digits (not bits) to the right.
 */
bool sbn_shr_d (struct sbn * a, size_t n)
{
	if (!a) return false;
	if (n == 0) return true;
	size_t ndigs = sbn_ndigits(a);
	if (ndigs <= n) return _sbn_flush_digits(a), true;
	/* TODO: Optimize this */
	for (; n > 0; n--) _sbn_digits_vec_remove(a->digits, 0);
	return true;
}

bool sbn_shr (struct sbn * r, const struct sbn * a, size_t n)
{ return sbn_clone_to(r, a) && sbn_shr_d(r, n); }

bool sbn_mul_digit_ud (struct sbn * a, const sbn_digit d)
{
	if (!a) return false;
	if (sbn_is_zero(a)) return true;
	if (d == 0) return _sbn_flush_digits(a), true;

	size_t ndigs = sbn_ndigits(a);
	sbn_digit carry = 0;
	bool ret = true;
	for (size_t i = 0; ret && i < ndigs; i++)
		ret = _sbn_set_nth_digit(a, i, _sbn_digit_mul(sbn_nth_digit(a, i), d, &carry));

	return ret && (!carry || _sbn_push_digit(a, carry));
}

bool sbn_mul_digit_u (struct sbn * r, const struct sbn * a, const sbn_digit d)
{
	if (!a || !r) return false;
	if (d == 0) return _sbn_flush_digits(r), true;
	if (!sbn_clone_to(r, a)) return _sbn_flush_digits(r), false;
	return sbn_mul_digit_ud(r, d);
}

bool sbn_mul_u (struct sbn * r, const struct sbn * a, const struct sbn * b)
{
	if (!a || !b) return false;

	size_t andigs = sbn_ndigits(a);
	size_t bndigs = sbn_ndigits(b);

	if (sbn_is_zero(a) || sbn_is_zero(b)) return _sbn_flush_digits(r), true;

	/* Traversing the number of fewer digits results in fewer operations */
	if (andigs < bndigs) {
		{ const struct sbn * tmp = a; a = b; b = tmp; }
		{ size_t tmp = andigs; andigs = bndigs; bndigs = tmp; }
	}

	if (!_sbn_flush_digits(r) || !_sbn_reserve(r, andigs+bndigs-1)) return false;

	struct sbn c[1] = {0};
	struct sbn d[1] = {0};

	/* MCA§1.3.1 Algorithm 1.2 BasecaseMultiply */
	bool ret = sbn_mul_digit_u(r, a, sbn_nth_digit(b, 0)); /* r <- a*b0 */

	for (size_t i = 1; ret && i < bndigs; i++)
		ret = sbn_mul_digit_u(c, a, sbn_nth_digit(b, i)) /* a*bi */
			/* TODO: avoid shift -- specialized add starting at index? */
			&& sbn_shl_d(c, i) /* B^i*(a*bi) */
			/* TODO: optimize sbn_add_ud to modify r in-place */
			&& sbn_add_u(d, r, c) /* r + B^i*(a*bi) */
			&& sbn_clone_to(r, d); /* r <- r + B^i*(a*bi) */

	sbn_destroy(c);
	sbn_destroy(d);

	return ret || (_sbn_flush_digits(r), false);
}

bool sbn_mul (struct sbn * r, const struct sbn * a, const struct sbn * b)
{
	if (sbn_mul_u(r, a, b)) {
		/* Resulting sign is negative if the two operands' signs are different */
		sbn_set_sign(r, sbn_is_negative(a) != sbn_is_negative(b));
		return true;
	}
	return false;
}

/*
 * Implementation clean up
 */

#undef SBN_CFG_IMPLEMENTATION
#undef SBN_CFG_NO_STDINT

#endif /* SBN_CFG_IMPLEMENTATION */
