#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/color.h>

static void test_chess_color_is_valid(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessColor color;
		bool is_valid;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
		{ .color = CHESS_COLOR_NONE, .is_valid = false },

		{ .color = CHESS_COLOR_WHITE, .is_valid = true },
		{ .color = CHESS_COLOR_BLACK, .is_valid = true },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		bool is_valid = chess_color_is_valid(test_cases[i].color);
		if (test_cases[i].is_valid) {
			assert_true(is_valid);
		} else {
			assert_false(is_valid);
		}
	}
}
static void test_chess_color_opposite(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessColor color;
		ChessColor opposite;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
		{ .color = CHESS_COLOR_WHITE, .opposite = CHESS_COLOR_BLACK },
		{ .color = CHESS_COLOR_BLACK, .opposite = CHESS_COLOR_WHITE },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessColor opposite = chess_color_opposite(test_cases[i].color);
		assert_int_equal(opposite, test_cases[i].opposite);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_color_is_valid),
		cmocka_unit_test(test_chess_color_opposite),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
