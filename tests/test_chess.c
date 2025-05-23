#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess.h>

static void test_filter(void **state) {
	(void)state;

	assert_int_equal(1 + 1, 2);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_filter),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
