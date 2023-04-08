#define SBN_CFG_IMPLEMENTATION
#include <utils/sbn.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define debug_log(msg, ...) fprintf(stderr, "%s:%s():%u: " msg "\n", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define bool2str(b) ((b) ? "true" : "false")

void print_digs (const struct sbn * a)
{
	size_t ndigs = sbn_ndigits(a);
	for (size_t i = 0; i < ndigs; i++)
		debug_log("digit_%zu=%u", i, sbn_nth_digit(a, i));
}

/* Adding digits to an SBN, adding SBNs, and sbn->string */
void func1 (void)
{
	struct sbn sbns[3] = {{0}};
	struct sbn * a = sbns + 0;
	struct sbn * b = sbns + 1;
	struct sbn * c = sbns + 2;

	sbn_digit elem = ~0U;
	debug_log("elem=%u", elem);
	for (size_t i = 0; i < 10; i++)
		assert(sbn_add_digit_ud(a, elem));

	debug_log("final ndigs=%zu", sbn_ndigits(a));

	char * astr = sbn_to_str(a, 16);
	assert(astr != NULL);
	assert(strcmp(astr, "9fffffff6") == 0);
	free(astr);

	assert(sbn_clone_to(b, a));
	assert(sbn_add(c, a, b));

	char * cstr = sbn_to_str_16(c);
	assert(cstr != NULL);
	assert(strcmp(cstr, "13ffffffec") == 0);
	free(cstr);

	sbn_destroy(c);
	sbn_destroy(b);
	sbn_destroy(a);
}

void func2 (void)
{
	const char from_str[] = "9";
	sbn_digit dig = 0;
	size_t processed = 0;
	assert(_sbn_digit_from_str_16(strlen(from_str), from_str, &dig, &processed));
}

/* string->sbn */
void func3 (void)
{
	struct sbn a[1] = {{0}};
	const char from_str[] = "9fffffff6";
	assert(sbn_from_str_16(a, strlen(from_str), from_str));
	debug_log("str->sbn(%p)", (void*) a);
	char * to_str = sbn_to_str_16(a);
	assert(to_str != NULL);
	assert(strcmp(from_str, to_str) == 0);
	free(to_str);
	sbn_destroy(a);
}

void func4 (void)
{
	struct sbn a[1] = {{0}};
	const char from_str[] = "9fffffff6";
	assert(sbn_from_str_16(a, strlen(from_str), from_str));
	struct sbn * b = sbn_mul_digit_u(a, 16);
	assert(b != NULL);
	char * bstr = sbn_to_str_16(b);
	assert(bstr != NULL);
	debug_log("b=a*16=%s", bstr);
	bstr = (free(bstr), NULL);
	b = sbn_free(b);
	sbn_destroy(a);
}

/* Subtracting two numbers */
void func5 (void)
{
	struct sbn a[1] = {0};
	const char astr[] = "100000000000000000000"; /* 2^80 */
	const char bstr[] = "2000000000000000000"; /* 2^73 */
	const char expected[] = "fe000000000000000000";

	assert(sbn_from_str_16(a, strlen(astr), astr));
	struct sbn b[1] = {0};
	assert(sbn_from_str_16(b, strlen(bstr), bstr));
	struct sbn c[1] = {0};
	assert(sbn_sub(c, a, b));
	char * cstr = sbn_to_str_16(c);
	assert(cstr != NULL);
	debug_log("cstr=\"%s\"", cstr);
	debug_log("expc=\"%s\"", expected);
	assert(strcmp(cstr, expected) == 0);
	free(cstr);
	sbn_destroy(b);
	sbn_destroy(a);
}

/* string->sbn */
void func6 (void)
{
	struct sbn a[1] = {{0}};
	const char from_str[] = "8fa2a17d7b5601878aaeb79a44411ec3c31238e1c3017ff33c23a64013300c9caeb6582af97fb0ed27ab316ee493d6c32cde17ed00e4a3115de5e676307ab17477e59f853fefb662a21c3048e6e6a173c33ce9e8bbcbc58d438f41353f92ab32eabef0ebde6ad3c00712c81b49b7266f33556935e9aaa9a7139c46116d3a2b282a73716a12ebc74eb2fbc17fe98d494dcf863e7c9ee8ea90183f89139febd16339d08b823665374e817a52f094782f2722581651dfdb3ed51f4fb56b4993463ee69997325f90e7ec8f0ecbf81ecee299475ba59ea01719ce7136a1e5f6fdbfefacf3c568763eaa9ba5fdd9b1850f4eb0c5e4b63d0ff80ea821379be444db804cb2cc22aedb0db91cb11187c2987c1c1ab5b2bd6891911f7d85f2d457fc3daf7a9fe7d2a359418fcc0c7a6fed8b1785b09964285b94a21b2da3de0376c0fecd3da230d6cd350e873163ecb7294bf3f3dee51d9dd5b10fba4a8919bd16a4a3c1498011a808c7bfb051bbf7a6c8a16db5e4078dd8e6e2c141";
	assert(sbn_from_str_16(a, strlen(from_str), from_str));
	debug_log("str->sbn(%p)", (void*) a);
	char * to_str = sbn_to_str_16(a);
	assert(to_str != NULL);
	assert(strcmp(from_str, to_str) == 0);
	free(to_str);
	sbn_destroy(a);
}

void run_func (void (* func) (void), const char name[])
{
	printf("### %s ###\n", name); func(); printf("\n");
}

int main (void)
{
#define run(func) run_func(func, #func)

	run(func1);
	run(func2);
	run(func3);
	run(func4);
	run(func5);
	run(func6);

	return 0;
}
