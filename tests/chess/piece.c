#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/piece.h>

static void test_chess_piece_new(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessColor color;
		ChessPieceType type;
		ChessPiece piece;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
		{ .color = CHESS_COLOR_NONE, .type = CHESS_PIECE_TYPE_NONE, .piece = CHESS_PIECE_NONE },

		{ .color = CHESS_COLOR_WHITE, .type = CHESS_PIECE_TYPE_PAWN, .piece = CHESS_PIECE_WHITE_PAWN },
		{ .color = CHESS_COLOR_WHITE, .type = CHESS_PIECE_TYPE_KNIGHT, .piece = CHESS_PIECE_WHITE_KNIGHT },
		{ .color = CHESS_COLOR_WHITE, .type = CHESS_PIECE_TYPE_BISHOP, .piece = CHESS_PIECE_WHITE_BISHOP },
		{ .color = CHESS_COLOR_WHITE, .type = CHESS_PIECE_TYPE_ROOK, .piece = CHESS_PIECE_WHITE_ROOK },
		{ .color = CHESS_COLOR_WHITE, .type = CHESS_PIECE_TYPE_QUEEN, .piece = CHESS_PIECE_WHITE_QUEEN },
		{ .color = CHESS_COLOR_WHITE, .type = CHESS_PIECE_TYPE_KING, .piece = CHESS_PIECE_WHITE_KING },

		{ .color = CHESS_COLOR_BLACK, .type = CHESS_PIECE_TYPE_PAWN, .piece = CHESS_PIECE_BLACK_PAWN },
		{ .color = CHESS_COLOR_BLACK, .type = CHESS_PIECE_TYPE_KNIGHT, .piece = CHESS_PIECE_BLACK_KNIGHT },
		{ .color = CHESS_COLOR_BLACK, .type = CHESS_PIECE_TYPE_BISHOP, .piece = CHESS_PIECE_BLACK_BISHOP },
		{ .color = CHESS_COLOR_BLACK, .type = CHESS_PIECE_TYPE_ROOK, .piece = CHESS_PIECE_BLACK_ROOK },
		{ .color = CHESS_COLOR_BLACK, .type = CHESS_PIECE_TYPE_QUEEN, .piece = CHESS_PIECE_BLACK_QUEEN },
		{ .color = CHESS_COLOR_BLACK, .type = CHESS_PIECE_TYPE_KING, .piece = CHESS_PIECE_BLACK_KING },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessPiece piece = chess_piece_new(test_cases[i].color, test_cases[i].type);
		assert_int_equal(piece, test_cases[i].piece);
	}
}
static void test_chess_piece_color(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessPiece piece;
		ChessColor color;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
		{ .piece = CHESS_PIECE_NONE, .color = CHESS_COLOR_NONE },

		{ .piece = CHESS_PIECE_WHITE_PAWN, .color = CHESS_COLOR_WHITE },
		{ .piece = CHESS_PIECE_WHITE_KNIGHT, .color = CHESS_COLOR_WHITE },
		{ .piece = CHESS_PIECE_WHITE_BISHOP, .color = CHESS_COLOR_WHITE },
		{ .piece = CHESS_PIECE_WHITE_ROOK, .color = CHESS_COLOR_WHITE },
		{ .piece = CHESS_PIECE_WHITE_QUEEN, .color = CHESS_COLOR_WHITE },
		{ .piece = CHESS_PIECE_WHITE_KING, .color = CHESS_COLOR_WHITE },

		{ .piece = CHESS_PIECE_BLACK_PAWN, .color = CHESS_COLOR_BLACK },
		{ .piece = CHESS_PIECE_BLACK_KNIGHT, .color = CHESS_COLOR_BLACK },
		{ .piece = CHESS_PIECE_BLACK_BISHOP, .color = CHESS_COLOR_BLACK },
		{ .piece = CHESS_PIECE_BLACK_ROOK, .color = CHESS_COLOR_BLACK },
		{ .piece = CHESS_PIECE_BLACK_QUEEN, .color = CHESS_COLOR_BLACK },
		{ .piece = CHESS_PIECE_BLACK_KING, .color = CHESS_COLOR_BLACK },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessColor color = chess_piece_color(test_cases[i].piece);
		assert_int_equal(color, test_cases[i].color);
	}
}
static void test_chess_piece_type(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessPiece piece;
		ChessPieceType type;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
		{ .piece = CHESS_PIECE_NONE, .type = CHESS_PIECE_TYPE_NONE },

		{ .piece = CHESS_PIECE_WHITE_PAWN, .type = CHESS_PIECE_TYPE_PAWN },
		{ .piece = CHESS_PIECE_WHITE_KNIGHT, .type = CHESS_PIECE_TYPE_KNIGHT },
		{ .piece = CHESS_PIECE_WHITE_BISHOP, .type = CHESS_PIECE_TYPE_BISHOP },
		{ .piece = CHESS_PIECE_WHITE_ROOK, .type = CHESS_PIECE_TYPE_ROOK },
		{ .piece = CHESS_PIECE_WHITE_QUEEN, .type = CHESS_PIECE_TYPE_QUEEN },
		{ .piece = CHESS_PIECE_WHITE_KING, .type = CHESS_PIECE_TYPE_KING },

		{ .piece = CHESS_PIECE_BLACK_PAWN, .type = CHESS_PIECE_TYPE_PAWN },
		{ .piece = CHESS_PIECE_BLACK_KNIGHT, .type = CHESS_PIECE_TYPE_KNIGHT },
		{ .piece = CHESS_PIECE_BLACK_BISHOP, .type = CHESS_PIECE_TYPE_BISHOP },
		{ .piece = CHESS_PIECE_BLACK_ROOK, .type = CHESS_PIECE_TYPE_ROOK },
		{ .piece = CHESS_PIECE_BLACK_QUEEN, .type = CHESS_PIECE_TYPE_QUEEN },
		{ .piece = CHESS_PIECE_BLACK_KING, .type = CHESS_PIECE_TYPE_KING },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessPieceType type = chess_piece_type(test_cases[i].piece);
		assert_int_equal(type, test_cases[i].type);
	}
}
static void test_chess_piece_is_valid(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessPiece piece;
		bool is_valid;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
		{ .piece = CHESS_PIECE_NONE, .is_valid = false },

		{ .piece = CHESS_PIECE_WHITE_PAWN, .is_valid = true },
		{ .piece = CHESS_PIECE_WHITE_KNIGHT, .is_valid = true },
		{ .piece = CHESS_PIECE_WHITE_BISHOP, .is_valid = true },
		{ .piece = CHESS_PIECE_WHITE_ROOK, .is_valid = true },
		{ .piece = CHESS_PIECE_WHITE_QUEEN, .is_valid = true },
		{ .piece = CHESS_PIECE_WHITE_KING, .is_valid = true },

		{ .piece = CHESS_PIECE_BLACK_PAWN, .is_valid = true },
		{ .piece = CHESS_PIECE_BLACK_KNIGHT, .is_valid = true },
		{ .piece = CHESS_PIECE_BLACK_BISHOP, .is_valid = true },
		{ .piece = CHESS_PIECE_BLACK_ROOK, .is_valid = true },
		{ .piece = CHESS_PIECE_BLACK_QUEEN, .is_valid = true },
		{ .piece = CHESS_PIECE_BLACK_KING, .is_valid = true },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		bool is_valid = chess_piece_is_valid(test_cases[i].piece);
		if (test_cases[i].is_valid) {
			assert_true(is_valid);
		} else {
			assert_false(is_valid);
		}
	}
}
static void test_chess_piece_from_algebraic(void **state) {
	(void)state;

	typedef struct TestCase {
		const char *string;
		ChessPiece piece;
		size_t read;
	} TestCase;

	static const TestCase test_cases[] = {
		{ .string = "P", .piece = CHESS_PIECE_WHITE_PAWN, .read = 1 },
		{ .string = "N", .piece = CHESS_PIECE_WHITE_KNIGHT, .read = 1 },
		{ .string = "B", .piece = CHESS_PIECE_WHITE_BISHOP, .read = 1 },
		{ .string = "R", .piece = CHESS_PIECE_WHITE_ROOK, .read = 1 },
		{ .string = "Q", .piece = CHESS_PIECE_WHITE_QUEEN, .read = 1 },
		{ .string = "K", .piece = CHESS_PIECE_WHITE_KING, .read = 1 },

		{ .string = "p", .piece = CHESS_PIECE_BLACK_PAWN, .read = 1 },
		{ .string = "n", .piece = CHESS_PIECE_BLACK_KNIGHT, .read = 1 },
		{ .string = "b", .piece = CHESS_PIECE_BLACK_BISHOP, .read = 1 },
		{ .string = "r", .piece = CHESS_PIECE_BLACK_ROOK, .read = 1 },
		{ .string = "q", .piece = CHESS_PIECE_BLACK_QUEEN, .read = 1 },
		{ .string = "k", .piece = CHESS_PIECE_BLACK_KING, .read = 1 },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessPiece piece = CHESS_PIECE_NONE;
		size_t read      = chess_piece_from_algebraic(&piece, test_cases[i].string);
		assert_int_equal(piece, test_cases[i].piece);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_piece_to_algebraic(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessPiece piece;
		const char *string;
		size_t written;
	} TestCase;

	static const TestCase test_cases[] = {
		{ .piece = CHESS_PIECE_WHITE_PAWN, .string = "P", .written = 1 },
		{ .piece = CHESS_PIECE_WHITE_KNIGHT, .string = "N", .written = 1 },
		{ .piece = CHESS_PIECE_WHITE_BISHOP, .string = "B", .written = 1 },
		{ .piece = CHESS_PIECE_WHITE_ROOK, .string = "R", .written = 1 },
		{ .piece = CHESS_PIECE_WHITE_QUEEN, .string = "Q", .written = 1 },
		{ .piece = CHESS_PIECE_WHITE_KING, .string = "K", .written = 1 },

		{ .piece = CHESS_PIECE_BLACK_PAWN, .string = "p", .written = 1 },
		{ .piece = CHESS_PIECE_BLACK_KNIGHT, .string = "n", .written = 1 },
		{ .piece = CHESS_PIECE_BLACK_BISHOP, .string = "b", .written = 1 },
		{ .piece = CHESS_PIECE_BLACK_ROOK, .string = "r", .written = 1 },
		{ .piece = CHESS_PIECE_BLACK_QUEEN, .string = "q", .written = 1 },
		{ .piece = CHESS_PIECE_BLACK_KING, .string = "k", .written = 1 },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		char string[2] = { 0 };
		size_t written = chess_piece_to_algebraic(test_cases[i].piece, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_piece_new),
		cmocka_unit_test(test_chess_piece_is_valid),
		cmocka_unit_test(test_chess_piece_color),
		cmocka_unit_test(test_chess_piece_type),
		cmocka_unit_test(test_chess_piece_from_algebraic),
		cmocka_unit_test(test_chess_piece_to_algebraic),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
