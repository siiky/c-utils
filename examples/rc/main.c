#include "rc_int.h"

int main (void)
{
	struct rc_int rc[1] = {rc_int_new(10)};
	rc_int_free(rc);
	return 0;
}
