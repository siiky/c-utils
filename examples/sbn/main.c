#include <stdio.h>
#define debug_log(msg, ...) fprintf(stderr, "%s:%s():%u: " msg, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define bool_to_str(b) ((b) ? "true" : "false")

#define SBN_CFG_IMPLEMENTATION
#include <utils/sbn.h>

#include <assert.h>
#include <stdlib.h>

void print_digs (const struct sbn * a)
{
	size_t ndigs = sbn_ndigits(a);
	for (size_t i = 0; i < ndigs; i++)
		debug_log("digit_%zu=%u\n", i, sbn_nth_digit(a, i));
}

int main (void)
{
	{
		sbn_digit a = 0x12634978;
		sbn_digit b = 0x12634979;
		sbn_double_digit c = ((sbn_double_digit) a) + sbn_twos_compl((sbn_double_digit) b);

		assert(c == (a - b));
		debug_log("a=0x%x, b=0x%x, c=0x%lx\n", a, b, c);
	}

	{
		struct sbn * a = sbn_new();

		if (a) {
			sbn_digit elem = ~0U;
			debug_log("elem=%u\n", elem);
			for (size_t i = 0; i < 10; i++) {
				debug_log("i=%zu,\tndigs=%zu\n", i, sbn_ndigits(a));
				print_digs(a);
				puts("");

				struct sbn * tmp = sbn_add_digit_u(a, elem);
				debug_log("tmp=%p\n", (void*) tmp);
				if (tmp) {
					a = sbn_free(a);
					a = tmp;
				}
			}

			debug_log("final ndigs=%zu\n", sbn_ndigits(a));
			print_digs(a);

			char * str = sbn_to_str(a, 16);
			if (str) {
				debug_log("str(%p)=%s\n", (void*) str, str);
				free(str);
			}

			struct sbn * b = sbn_clone(a);
			if (b) {

			}

			a = sbn_free(a);
		}
	}

	return 0;
}
