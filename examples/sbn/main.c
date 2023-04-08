#define SBN_CFG_IMPLEMENTATION
#include <utils/sbn.h>

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
	for (size_t i = 0; i < 10; i++) {
		puts("");
		debug_log("i=%zu,\tndigs=%zu", i, sbn_ndigits(a));
		//print_digs(a);

		bool succ = sbn_add_digit_ud(a, elem);
		debug_log("succ=%s", bool2str(succ));
	}

	debug_log("final ndigs=%zu", sbn_ndigits(a));
	print_digs(a);

	char * astr = sbn_to_str(a, 16);
	if (astr) {
		if (strcmp(astr, "9fffffff6"))
			debug_log("UNEXPECTED RESULT! astr(%p)=%s", (void*) astr, astr);
		free(astr);
	}

	if (!sbn_clone_to(b, a)) debug_log("CLONE FAILED!%s", "");
	if (!sbn_add(c, a, b)) debug_log("ADD FAILED!%s", "");

	char * cstr = sbn_to_str_16(c);
	if (cstr) {
		if (strcmp(cstr, "13ffffffec"))
			debug_log("UNEXPECTED RESULT! cstr(%p)=%s", (void*) cstr, cstr);
		free(cstr);
	}
	print_digs(c);

	sbn_destroy(c);
	sbn_destroy(b);
	sbn_destroy(a);
}

void func2 (void)
{
	const char from_str[] = "9";
	sbn_digit dig = 0;
	size_t processed = 0;
	if (_sbn_digit_from_str_16(strlen(from_str), from_str, &dig, &processed)) {
		debug_log("str=\"%s\" -> dig=%u, processed=%zu\n", from_str, dig, processed);
	}
}

/* string->sbn */
void func3 (void)
{
	struct sbn a[1] = {{0}};
	const char from_str[] = "9fffffff6";
	if (sbn_from_str_16(a, strlen(from_str), from_str)) {
		debug_log("str->sbn(%p)", (void*) a);
		print_digs(a);
		char * to_str = sbn_to_str_16(a);
		if (to_str) {
			if (strcmp(from_str, to_str))
				debug_log("Resulting string (%s) differs from original string (%s)!", to_str, from_str);
			free(to_str);
		}
		sbn_destroy(a);
	}
}

void func4 (void)
{
	struct sbn a[1] = {{0}};
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

/* Subtracting two numbers */
void func5 (void)
{
	struct sbn a[1] = {0};
	const char astr[] = "100000000000000000000"; /* 2^80 */
	const char bstr[] = "2000000000000000000"; /* 2^73 */
	const char expected[] = "fe000000000000000000";

	if (sbn_from_str_16(a, strlen(astr), astr)) {
		struct sbn b[1] = {0};
		if (sbn_from_str_16(b, strlen(bstr), bstr)) {
			struct sbn c[1] = {0};
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

/* string->sbn */
void func6 (void)
{
	struct sbn a[1] = {{0}};
	const char from_str[] = "8fa2a17d7b5601878aaeb79a44411ec3c31238e1c3017ff33c23a64013300c9caeb6582af97fb0ed27ab316ee493d6c32cde17ed00e4a3115de5e676307ab17477e59f853fefb662a21c3048e6e6a173c33ce9e8bbcbc58d438f41353f92ab32eabef0ebde6ad3c00712c81b49b7266f33556935e9aaa9a7139c46116d3a2b282a73716a12ebc74eb2fbc17fe98d494dcf863e7c9ee8ea90183f89139febd16339d08b823665374e817a52f094782f2722581651dfdb3ed51f4fb56b4993463ee69997325f90e7ec8f0ecbf81ecee299475ba59ea01719ce7136a1e5f6fdbfefacf3c568763eaa9ba5fdd9b1850f4eb0c5e4b63d0ff80ea821379be444db804cb2cc22aedb0db91cb11187c2987c1c1ab5b2bd6891911f7d85f2d457fc3daf7a9fe7d2a359418fcc0c7a6fed8b1785b09964285b94a21b2da3de0376c0fecd3da230d6cd350e873163ecb7294bf3f3dee51d9dd5b10fba4a8919bd16a4a3c1498011a808c7bfb051bbf7a6c8a16db5e4078dd8e6e2c141";
	if (sbn_from_str_16(a, strlen(from_str), from_str)) {
		debug_log("str->sbn(%p)", (void*) a);
		print_digs(a);
		char * to_str = sbn_to_str_16(a);
		if (to_str) {
			if (strcmp(from_str, to_str))
				debug_log("Resulting string (%s) differs from original string (%s)!", to_str, from_str);
			free(to_str);
		}
		sbn_destroy(a);
	}
}

int main (void)
{
	//func1();
	//func2();
	//func3();
	//func4();
	//func5();
	func6();

	return 0;
}
