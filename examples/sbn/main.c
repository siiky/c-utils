#include <utils/sbn.h>

#include <assert.h>
#include <stdio.h>

int main (void)
{
	sbn_digit a = 0x12634978;
	sbn_digit b = 0x12634979;
	sbn_double_digit c = ((sbn_double_digit) a) + sbn_twos_compl((sbn_double_digit) b);

	assert(c == (a - b));
	printf("a=0x%x, b=0x%x, c=0x%lx\n", a, b, c);

	return 0;
}
