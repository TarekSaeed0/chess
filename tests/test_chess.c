#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess.h>

static void test_chess_preft(void **state) {
	(void)state;

	unsigned long expected[] = {
		1,
		20,
		400,
		8902,
		197281
	};

	for (unsigned int depth = 0; depth < sizeof(expected) / sizeof(expected[0]); depth++) {
		assert_int_equal(chess_perft(depth), expected[depth]);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_preft),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
