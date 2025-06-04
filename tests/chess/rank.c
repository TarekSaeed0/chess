#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/rank.h>

static void test_chess_rank_is_valid(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessRank rank;
		bool is_valid;
	} test_cases[] = {
		{ .rank = CHESS_RANK_NONE, .is_valid = false },

		{ .rank = CHESS_RANK_1, .is_valid = true },
		{ .rank = CHESS_RANK_2, .is_valid = true },
		{ .rank = CHESS_RANK_3, .is_valid = true },
		{ .rank = CHESS_RANK_4, .is_valid = true },
		{ .rank = CHESS_RANK_5, .is_valid = true },
		{ .rank = CHESS_RANK_6, .is_valid = true },
		{ .rank = CHESS_RANK_7, .is_valid = true },
		{ .rank = CHESS_RANK_8, .is_valid = true },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		bool is_valid = chess_rank_is_valid(test_cases[i].rank);
		if (test_cases[i].is_valid) {
			assert_true(is_valid);
		} else {
			assert_false(is_valid);
		}
	}
}
static void test_chess_rank_from_algebraic(void **state) {
	(void)state;

	static const struct {
		const char *string;
		ChessRank rank;
		size_t read;
	} test_cases[] = {
		{ .string = "1", .rank = CHESS_RANK_1, .read = 1 },
		{ .string = "2", .rank = CHESS_RANK_2, .read = 1 },
		{ .string = "3", .rank = CHESS_RANK_3, .read = 1 },
		{ .string = "4", .rank = CHESS_RANK_4, .read = 1 },
		{ .string = "5", .rank = CHESS_RANK_5, .read = 1 },
		{ .string = "6", .rank = CHESS_RANK_6, .read = 1 },
		{ .string = "7", .rank = CHESS_RANK_7, .read = 1 },
		{ .string = "8", .rank = CHESS_RANK_8, .read = 1 },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessRank rank = CHESS_RANK_NONE;
		size_t read    = chess_rank_from_algebraic(&rank, test_cases[i].string);
		assert_int_equal(rank, test_cases[i].rank);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_rank_to_algebraic(void **state) {
	(void)state;

	static const struct {
		ChessRank rank;
		const char *string;
		size_t written;
	} test_cases[] = {
		{ .rank = CHESS_RANK_1, .string = "1", .written = 1 },
		{ .rank = CHESS_RANK_2, .string = "2", .written = 1 },
		{ .rank = CHESS_RANK_3, .string = "3", .written = 1 },
		{ .rank = CHESS_RANK_4, .string = "4", .written = 1 },
		{ .rank = CHESS_RANK_5, .string = "5", .written = 1 },
		{ .rank = CHESS_RANK_6, .string = "6", .written = 1 },
		{ .rank = CHESS_RANK_7, .string = "7", .written = 1 },
		{ .rank = CHESS_RANK_8, .string = "8", .written = 1 },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		char string[2] = { 0 };
		size_t written = chess_rank_to_algebraic(test_cases[i].rank, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_rank_is_valid),
		cmocka_unit_test(test_chess_rank_from_algebraic),
		cmocka_unit_test(test_chess_rank_to_algebraic),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
