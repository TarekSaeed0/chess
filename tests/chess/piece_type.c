#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/piece_type.h>

static void test_chess_piece_type_is_valid(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessPieceType type;
		bool is_valid;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
		{ .type = CHESS_PIECE_TYPE_NONE, .is_valid = false },

		{ .type = CHESS_PIECE_TYPE_PAWN, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_KNIGHT, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_BISHOP, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_ROOK, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_QUEEN, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_KING, .is_valid = true },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		bool is_valid = chess_piece_type_is_valid(test_cases[i].type);
		if (test_cases[i].is_valid) {
			assert_true(is_valid);
		} else {
			assert_false(is_valid);
		}
	}
}
static void test_chess_piece_type_from_algebraic(void **state) {
	(void)state;

	typedef struct TestCase {
		const char *string;
		ChessPieceType type;
		size_t read;
	} TestCase;

	static const TestCase test_cases[] = {
		{ .string = "P", .type = CHESS_PIECE_TYPE_PAWN, .read = 1 },
		{ .string = "N", .type = CHESS_PIECE_TYPE_KNIGHT, .read = 1 },
		{ .string = "B", .type = CHESS_PIECE_TYPE_BISHOP, .read = 1 },
		{ .string = "R", .type = CHESS_PIECE_TYPE_ROOK, .read = 1 },
		{ .string = "Q", .type = CHESS_PIECE_TYPE_QUEEN, .read = 1 },
		{ .string = "K", .type = CHESS_PIECE_TYPE_KING, .read = 1 },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessPieceType type = CHESS_PIECE_TYPE_NONE;
		size_t read         = chess_piece_type_from_algebraic(&type, test_cases[i].string);
		assert_int_equal(type, test_cases[i].type);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_piece_type_to_algebraic(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessPieceType type;
		const char *string;
		size_t written;
	} TestCase;

	static const TestCase test_cases[] = {
		{ .type = CHESS_PIECE_TYPE_PAWN, .string = "P", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_KNIGHT, .string = "N", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_BISHOP, .string = "B", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_ROOK, .string = "R", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_QUEEN, .string = "Q", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_KING, .string = "K", .written = 1 },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		char string[2] = { 0 };
		size_t written = chess_piece_type_to_algebraic(test_cases[i].type, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_piece_type_is_valid),
		cmocka_unit_test(test_chess_piece_type_from_algebraic),
		cmocka_unit_test(test_chess_piece_type_to_algebraic),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
