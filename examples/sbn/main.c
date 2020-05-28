#include <utils/sbn.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	{
		sbn_digit a = 0x12634978;
		sbn_digit b = 0x12634979;
		sbn_double_digit c = ((sbn_double_digit) a) + sbn_twos_compl((sbn_double_digit) b);

		assert(c == (a - b));
		printf("a=0x%x, b=0x%x, c=0x%lx\n", a, b, c);
	}

	{
		struct sbn * a = sbn_new();

		if (a) {
			for (size_t i = 0; i < 10; i++) {
				struct sbn * tmp = sbn_add_digit_u(a, ~0U);
				if (tmp) {
					sbn_free(a);
					a = tmp;
				}
			}

			char * str = sbn_to_str(a, 16);
			if (str)
				puts(str);

			free(str);
			sbn_free(a);
		}
	}

	return 0;
}
