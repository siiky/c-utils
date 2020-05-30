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
		struct sbn * a = sbn_new();
		if (a) {
			sbn_digit elem = ~0U;
			debug_log("elem=%u", elem);
			for (size_t i = 0; i < 10; i++) {
				debug_log("i=%zu,\tndigs=%zu", i, sbn_ndigits(a));
				print_digs(a);
				puts("");

				struct sbn * tmp = sbn_add_digit_u(a, elem);
				debug_log("tmp=%p", (void*) tmp);
				if (tmp) {
					a = sbn_free(a);
					a = tmp;
				}
			}

			debug_log("final ndigs=%zu", sbn_ndigits(a));
			print_digs(a);

			char * astr = sbn_to_str(a, 16);
			if (astr) {
				debug_log("astr(%p)=%s", (void*) astr, astr);
				free(astr);
			}

			struct sbn * b = sbn_clone(a);
			if (b) {
				struct sbn * c = sbn_add(a, b);
				if (c) {
					char * cstr = sbn_to_str_16(c);
					if (cstr) {
						debug_log("cstr(%p)=%s", (void*) cstr, cstr);
						free(cstr);
					}
					print_digs(c);
					c = sbn_free(c);
				}
				b = sbn_free(b);
			}
			a = sbn_free(a);
		}
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

#if 1
	{ /* string->sbn */
		const char from_str[] = "9fffffff6";
		struct sbn * a = sbn_from_str_16(0, from_str);
		if (a) {
			debug_log("str->sbn(%p)", (void*) a);
			print_digs(a);
			char * to_str = sbn_to_str_16(a);
			if (to_str) {
				debug_log("str->sbn->str = %s", to_str);
				free(to_str);
			}
			a = sbn_free(a);
		}
	}
#endif

	return 0;
}
