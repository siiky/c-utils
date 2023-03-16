#include <stdio.h>
#define debug_log(msg, ...) fprintf(stderr, "%s:%s():%u: " msg "\n", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define bool_to_str(b) ((b) ? "true" : "false")

#define SBN_CFG_IMPLEMENTATION
#include <utils/sbn.h>

#include <stdlib.h>

void print_digs (const struct sbn * a)
{
	size_t ndigs = sbn_ndigits(a);
	for (size_t i = 0; i < ndigs; i++)
		debug_log("digit_%zu=%u", i, sbn_nth_digit(a, i));
}

int main (void)
{
#if 0
	{ /* Adding digits to an SBN, adding SBNs, and sbn->string */
		struct sbn sbns[3];
		struct sbn * a = sbns + 0;
		struct sbn * b = sbns + 1;
		struct sbn * c = sbns + 2;

		sbn_digit elem = ~0U;
		debug_log("elem=%u", elem);
		for (size_t i = 0; i < 10; i++) {
			puts("");
			debug_log("i=%zu,\tndigs=%zu", i, sbn_ndigits(a));
			print_digs(a);

			bool succ = sbn_add_digit_ud(a, elem);
			debug_log("succ=%s", succ ? "true" : "false");
		}

		debug_log("final ndigs=%zu", sbn_ndigits(a));
		print_digs(a);

		char * astr = sbn_to_str(a, 16);
		if (astr) {
			debug_log("astr(%p)=%s", (void*) astr, astr);
			free(astr);
		}

		sbn_clone_to(b, a);
		sbn_add(c, a, b);
			char * cstr = sbn_to_str_16(c);
			if (cstr) {
				debug_log("cstr(%p)=%s", (void*) cstr, cstr);
				free(cstr);
			}
			print_digs(c);

		sbn_free(c);
		sbn_free(b);
		sbn_free(a);
	}
#endif

#if 0
	{
		const char from_str[] = "9";
		sbn_digit dig = 0;
		size_t processed = 0;
		if (_sbn_digit_from_str_16(strlen(from_str), from_str, &dig, &processed)) {
			debug_log("str=\"%s\" -> dig=%u, processed=%zu\n", from_str, dig, processed);
		}

	}
#endif

#if 0
	{ /* string->sbn */
		struct sbn a[1];
		const char from_str[] = "9fffffff6";
		if (sbn_from_str_16(a, strlen(from_str), from_str)) {
			debug_log("str->sbn(%p)", (void*) a);
			print_digs(a);
			char * to_str = sbn_to_str_16(a);
			if (to_str) {
				debug_log("str->sbn->str = %s", to_str);
				free(to_str);
			}
			sbn_destroy(a);
		}
	}
#endif

#if 0
	{
		struct sbn a[1];
		const char from_str[] = "9fffffff6";
		if (sbn_from_str_16(a, strlen(from_str), from_str)) {
			struct sbn * b = sbn_mul_digit_u(a, 16);
			if (b) {
				char * bstr = sbn_to_str_16(b);
				if (bstr) {
					debug_log("b=a*16=%s", bstr);
					bstr = (free(bstr), NULL);
				}
				b = sbn_free(b);
			} else {
				debug_log("failed to multiply %s", from_str);
			}
			sbn_destroy(a);
		}
	}
#endif

#if 1
	{ /* Subtracting two numbers */
		struct sbn a[1] = {0};
		struct sbn b[1] = {0};
		struct sbn c[1] = {0};
		const char astr[] = "100000000000000000000"; /* 2^80 */
		const char bstr[] = "2000000000000000000"; /* 2^73 */
		const char expected[] = "fe000000000000000000";

		if (sbn_from_str_16(a, strlen(astr), astr)) {
			if (sbn_from_str_16(b, strlen(bstr), bstr)) {
				if (sbn_sub(c, a, b)) {
					char * cstr = sbn_to_str_16(c);
					if (cstr) {
						printf("a=%s, b=%s, a-b=%s, got: %s\n",
							astr,
							bstr,
							expected,
							cstr
						);
					}
				}
				sbn_destroy(b);
			}
			sbn_destroy(a);
		}
	}
#endif

	return 0;
}
