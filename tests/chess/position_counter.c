#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/position_counter.h>

#include <chess/position.h>

static void test_chess_position_counter_is_valid(void **state) {
	(void)state;

	ChessPositionCounter counter = chess_position_counter_new();
	assert_true(chess_position_counter_is_valid(&counter));

	ChessPosition position = chess_position_new();
	chess_position_counter_increment(&counter, &position);
	assert_true(chess_position_counter_is_valid(&counter));

	chess_position_counter_increment(&counter, &position);
	assert_true(chess_position_counter_is_valid(&counter));

	chess_position_counter_decrement(&counter, &position);
	assert_true(chess_position_counter_is_valid(&counter));
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_position_counter_is_valid),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
