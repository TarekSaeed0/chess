#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

static void test_chess_color_is_valid(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessColor color;
		bool is_valid;
	} test_cases[] = {
		{ .color = CHESS_COLOR_NONE, .is_valid = false },

		{ .color = CHESS_COLOR_WHITE, .is_valid = true },
		{ .color = CHESS_COLOR_BLACK, .is_valid = true },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
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

	static CHESS_CONSTEXPR struct {
		ChessColor color;
		ChessColor opposite;
	} test_cases[] = {
		{ .color = CHESS_COLOR_WHITE, .opposite = CHESS_COLOR_BLACK },
		{ .color = CHESS_COLOR_BLACK, .opposite = CHESS_COLOR_WHITE },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessColor opposite = chess_color_opposite(test_cases[i].color);
		assert_int_equal(opposite, test_cases[i].opposite);
	}
}

static void test_chess_piece_type_is_valid(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessPieceType type;
		bool is_valid;
	} test_cases[] = {
		{ .type = CHESS_PIECE_TYPE_NONE, .is_valid = false },

		{ .type = CHESS_PIECE_TYPE_PAWN, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_KNIGHT, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_BISHOP, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_ROOK, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_QUEEN, .is_valid = true },
		{ .type = CHESS_PIECE_TYPE_KING, .is_valid = true },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
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

	static const struct {
		const char *string;
		ChessPieceType type;
		size_t read;
	} test_cases[] = {
		{ .string = "P", .type = CHESS_PIECE_TYPE_PAWN, .read = 1 },
		{ .string = "N", .type = CHESS_PIECE_TYPE_KNIGHT, .read = 1 },
		{ .string = "B", .type = CHESS_PIECE_TYPE_BISHOP, .read = 1 },
		{ .string = "R", .type = CHESS_PIECE_TYPE_ROOK, .read = 1 },
		{ .string = "Q", .type = CHESS_PIECE_TYPE_QUEEN, .read = 1 },
		{ .string = "K", .type = CHESS_PIECE_TYPE_KING, .read = 1 },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessPieceType type = CHESS_PIECE_TYPE_NONE;
		size_t read         = chess_piece_type_from_algebraic(&type, test_cases[i].string);
		assert_int_equal(type, test_cases[i].type);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_piece_type_to_algebraic(void **state) {
	(void)state;

	static const struct {
		ChessPieceType type;
		const char *string;
		size_t written;
	} test_cases[] = {
		{ .type = CHESS_PIECE_TYPE_PAWN, .string = "P", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_KNIGHT, .string = "N", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_BISHOP, .string = "B", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_ROOK, .string = "R", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_QUEEN, .string = "Q", .written = 1 },
		{ .type = CHESS_PIECE_TYPE_KING, .string = "K", .written = 1 },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		char string[2] = { 0 };
		size_t written = chess_piece_type_to_algebraic(test_cases[i].type, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

static void test_chess_piece_new(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessColor color;
		ChessPieceType type;
		ChessPiece piece;
	} test_cases[] = {
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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessPiece piece = chess_piece_new(test_cases[i].color, test_cases[i].type);
		assert_int_equal(piece, test_cases[i].piece);
	}
}
static void test_chess_piece_color(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessPiece piece;
		ChessColor color;
	} test_cases[] = {
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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessColor color = chess_piece_color(test_cases[i].piece);
		assert_int_equal(color, test_cases[i].color);
	}
}
static void test_chess_piece_type(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessPiece piece;
		ChessPieceType type;
	} test_cases[] = {
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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessPieceType type = chess_piece_type(test_cases[i].piece);
		assert_int_equal(type, test_cases[i].type);
	}
}
static void test_chess_piece_is_valid(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessPiece piece;
		bool is_valid;
	} test_cases[] = {
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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
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

	static const struct {
		const char *string;
		ChessPiece piece;
		size_t read;
	} test_cases[] = {
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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessPiece piece = CHESS_PIECE_NONE;
		size_t read      = chess_piece_from_algebraic(&piece, test_cases[i].string);
		assert_int_equal(piece, test_cases[i].piece);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_piece_to_algebraic(void **state) {
	(void)state;

	static const struct {
		ChessPiece piece;
		const char *string;
		size_t written;
	} test_cases[] = {
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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		char string[2] = { 0 };
		size_t written = chess_piece_to_algebraic(test_cases[i].piece, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

static void test_chess_file_is_valid(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessFile file;
		bool is_valid;
	} test_cases[] = {
		{ .file = CHESS_FILE_NONE, .is_valid = false },

		{ .file = CHESS_FILE_A, .is_valid = true },
		{ .file = CHESS_FILE_B, .is_valid = true },
		{ .file = CHESS_FILE_C, .is_valid = true },
		{ .file = CHESS_FILE_D, .is_valid = true },
		{ .file = CHESS_FILE_E, .is_valid = true },
		{ .file = CHESS_FILE_F, .is_valid = true },
		{ .file = CHESS_FILE_G, .is_valid = true },
		{ .file = CHESS_FILE_H, .is_valid = true },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		bool is_valid = chess_file_is_valid(test_cases[i].file);
		if (test_cases[i].is_valid) {
			assert_true(is_valid);
		} else {
			assert_false(is_valid);
		}
	}
}
static void test_chess_file_from_algebraic(void **state) {
	(void)state;

	static const struct {
		const char *string;
		ChessFile file;
		size_t read;
	} test_cases[] = {
		{ .string = "a", .file = CHESS_FILE_A, .read = 1 },
		{ .string = "b", .file = CHESS_FILE_B, .read = 1 },
		{ .string = "c", .file = CHESS_FILE_C, .read = 1 },
		{ .string = "d", .file = CHESS_FILE_D, .read = 1 },
		{ .string = "e", .file = CHESS_FILE_E, .read = 1 },
		{ .string = "f", .file = CHESS_FILE_F, .read = 1 },
		{ .string = "g", .file = CHESS_FILE_G, .read = 1 },
		{ .string = "h", .file = CHESS_FILE_H, .read = 1 },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessFile file = CHESS_FILE_NONE;
		size_t read    = chess_file_from_algebraic(&file, test_cases[i].string);
		assert_int_equal(file, test_cases[i].file);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_file_to_algebraic(void **state) {
	(void)state;

	static const struct {
		ChessFile file;
		const char *string;
		size_t written;
	} test_cases[] = {
		{ .file = CHESS_FILE_A, .string = "a", .written = 1 },
		{ .file = CHESS_FILE_B, .string = "b", .written = 1 },
		{ .file = CHESS_FILE_C, .string = "c", .written = 1 },
		{ .file = CHESS_FILE_D, .string = "d", .written = 1 },
		{ .file = CHESS_FILE_E, .string = "e", .written = 1 },
		{ .file = CHESS_FILE_F, .string = "f", .written = 1 },
		{ .file = CHESS_FILE_G, .string = "g", .written = 1 },
		{ .file = CHESS_FILE_H, .string = "h", .written = 1 },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		char string[2] = { 0 };
		size_t written = chess_file_to_algebraic(test_cases[i].file, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
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

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		char string[2] = { 0 };
		size_t written = chess_rank_to_algebraic(test_cases[i].rank, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

static void test_chess_square_new(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessFile file;
		ChessRank rank;
		ChessSquare square;
	} test_cases[] = {
		{ .file = CHESS_FILE_A, .rank = CHESS_RANK_1, .square = CHESS_SQUARE_A1 },
		{ .file = CHESS_FILE_B, .rank = CHESS_RANK_1, .square = CHESS_SQUARE_B1 },
		{ .file = CHESS_FILE_C, .rank = CHESS_RANK_1, .square = CHESS_SQUARE_C1 },
		{ .file = CHESS_FILE_D, .rank = CHESS_RANK_1, .square = CHESS_SQUARE_D1 },
		{ .file = CHESS_FILE_E, .rank = CHESS_RANK_1, .square = CHESS_SQUARE_E1 },
		{ .file = CHESS_FILE_F, .rank = CHESS_RANK_1, .square = CHESS_SQUARE_F1 },
		{ .file = CHESS_FILE_G, .rank = CHESS_RANK_1, .square = CHESS_SQUARE_G1 },
		{ .file = CHESS_FILE_H, .rank = CHESS_RANK_1, .square = CHESS_SQUARE_H1 },

		{ .file = CHESS_FILE_A, .rank = CHESS_RANK_2, .square = CHESS_SQUARE_A2 },
		{ .file = CHESS_FILE_B, .rank = CHESS_RANK_2, .square = CHESS_SQUARE_B2 },
		{ .file = CHESS_FILE_C, .rank = CHESS_RANK_2, .square = CHESS_SQUARE_C2 },
		{ .file = CHESS_FILE_D, .rank = CHESS_RANK_2, .square = CHESS_SQUARE_D2 },
		{ .file = CHESS_FILE_E, .rank = CHESS_RANK_2, .square = CHESS_SQUARE_E2 },
		{ .file = CHESS_FILE_F, .rank = CHESS_RANK_2, .square = CHESS_SQUARE_F2 },
		{ .file = CHESS_FILE_G, .rank = CHESS_RANK_2, .square = CHESS_SQUARE_G2 },
		{ .file = CHESS_FILE_H, .rank = CHESS_RANK_2, .square = CHESS_SQUARE_H2 },

		{ .file = CHESS_FILE_A, .rank = CHESS_RANK_3, .square = CHESS_SQUARE_A3 },
		{ .file = CHESS_FILE_B, .rank = CHESS_RANK_3, .square = CHESS_SQUARE_B3 },
		{ .file = CHESS_FILE_C, .rank = CHESS_RANK_3, .square = CHESS_SQUARE_C3 },
		{ .file = CHESS_FILE_D, .rank = CHESS_RANK_3, .square = CHESS_SQUARE_D3 },
		{ .file = CHESS_FILE_E, .rank = CHESS_RANK_3, .square = CHESS_SQUARE_E3 },
		{ .file = CHESS_FILE_F, .rank = CHESS_RANK_3, .square = CHESS_SQUARE_F3 },
		{ .file = CHESS_FILE_G, .rank = CHESS_RANK_3, .square = CHESS_SQUARE_G3 },
		{ .file = CHESS_FILE_H, .rank = CHESS_RANK_3, .square = CHESS_SQUARE_H3 },

		{ .file = CHESS_FILE_A, .rank = CHESS_RANK_4, .square = CHESS_SQUARE_A4 },
		{ .file = CHESS_FILE_B, .rank = CHESS_RANK_4, .square = CHESS_SQUARE_B4 },
		{ .file = CHESS_FILE_C, .rank = CHESS_RANK_4, .square = CHESS_SQUARE_C4 },
		{ .file = CHESS_FILE_D, .rank = CHESS_RANK_4, .square = CHESS_SQUARE_D4 },
		{ .file = CHESS_FILE_E, .rank = CHESS_RANK_4, .square = CHESS_SQUARE_E4 },
		{ .file = CHESS_FILE_F, .rank = CHESS_RANK_4, .square = CHESS_SQUARE_F4 },
		{ .file = CHESS_FILE_G, .rank = CHESS_RANK_4, .square = CHESS_SQUARE_G4 },
		{ .file = CHESS_FILE_H, .rank = CHESS_RANK_4, .square = CHESS_SQUARE_H4 },

		{ .file = CHESS_FILE_A, .rank = CHESS_RANK_5, .square = CHESS_SQUARE_A5 },
		{ .file = CHESS_FILE_B, .rank = CHESS_RANK_5, .square = CHESS_SQUARE_B5 },
		{ .file = CHESS_FILE_C, .rank = CHESS_RANK_5, .square = CHESS_SQUARE_C5 },
		{ .file = CHESS_FILE_D, .rank = CHESS_RANK_5, .square = CHESS_SQUARE_D5 },
		{ .file = CHESS_FILE_E, .rank = CHESS_RANK_5, .square = CHESS_SQUARE_E5 },
		{ .file = CHESS_FILE_F, .rank = CHESS_RANK_5, .square = CHESS_SQUARE_F5 },
		{ .file = CHESS_FILE_G, .rank = CHESS_RANK_5, .square = CHESS_SQUARE_G5 },
		{ .file = CHESS_FILE_H, .rank = CHESS_RANK_5, .square = CHESS_SQUARE_H5 },

		{ .file = CHESS_FILE_A, .rank = CHESS_RANK_6, .square = CHESS_SQUARE_A6 },
		{ .file = CHESS_FILE_B, .rank = CHESS_RANK_6, .square = CHESS_SQUARE_B6 },
		{ .file = CHESS_FILE_C, .rank = CHESS_RANK_6, .square = CHESS_SQUARE_C6 },
		{ .file = CHESS_FILE_D, .rank = CHESS_RANK_6, .square = CHESS_SQUARE_D6 },
		{ .file = CHESS_FILE_E, .rank = CHESS_RANK_6, .square = CHESS_SQUARE_E6 },
		{ .file = CHESS_FILE_F, .rank = CHESS_RANK_6, .square = CHESS_SQUARE_F6 },
		{ .file = CHESS_FILE_G, .rank = CHESS_RANK_6, .square = CHESS_SQUARE_G6 },
		{ .file = CHESS_FILE_H, .rank = CHESS_RANK_6, .square = CHESS_SQUARE_H6 },

		{ .file = CHESS_FILE_A, .rank = CHESS_RANK_7, .square = CHESS_SQUARE_A7 },
		{ .file = CHESS_FILE_B, .rank = CHESS_RANK_7, .square = CHESS_SQUARE_B7 },
		{ .file = CHESS_FILE_C, .rank = CHESS_RANK_7, .square = CHESS_SQUARE_C7 },
		{ .file = CHESS_FILE_D, .rank = CHESS_RANK_7, .square = CHESS_SQUARE_D7 },
		{ .file = CHESS_FILE_E, .rank = CHESS_RANK_7, .square = CHESS_SQUARE_E7 },
		{ .file = CHESS_FILE_F, .rank = CHESS_RANK_7, .square = CHESS_SQUARE_F7 },
		{ .file = CHESS_FILE_G, .rank = CHESS_RANK_7, .square = CHESS_SQUARE_G7 },
		{ .file = CHESS_FILE_H, .rank = CHESS_RANK_7, .square = CHESS_SQUARE_H7 },

		{ .file = CHESS_FILE_A, .rank = CHESS_RANK_8, .square = CHESS_SQUARE_A8 },
		{ .file = CHESS_FILE_B, .rank = CHESS_RANK_8, .square = CHESS_SQUARE_B8 },
		{ .file = CHESS_FILE_C, .rank = CHESS_RANK_8, .square = CHESS_SQUARE_C8 },
		{ .file = CHESS_FILE_D, .rank = CHESS_RANK_8, .square = CHESS_SQUARE_D8 },
		{ .file = CHESS_FILE_E, .rank = CHESS_RANK_8, .square = CHESS_SQUARE_E8 },
		{ .file = CHESS_FILE_F, .rank = CHESS_RANK_8, .square = CHESS_SQUARE_F8 },
		{ .file = CHESS_FILE_G, .rank = CHESS_RANK_8, .square = CHESS_SQUARE_G8 },
		{ .file = CHESS_FILE_H, .rank = CHESS_RANK_8, .square = CHESS_SQUARE_H8 },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessSquare square = chess_square_new(test_cases[i].file, test_cases[i].rank);
		assert_int_equal(square, test_cases[i].square);
	}
}
static void test_chess_square_file(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessSquare square;
		ChessFile file;
	} test_cases[] = {
		{ .square = CHESS_SQUARE_A1, .file = CHESS_FILE_A },
		{ .square = CHESS_SQUARE_B1, .file = CHESS_FILE_B },
		{ .square = CHESS_SQUARE_C1, .file = CHESS_FILE_C },
		{ .square = CHESS_SQUARE_D1, .file = CHESS_FILE_D },
		{ .square = CHESS_SQUARE_E1, .file = CHESS_FILE_E },
		{ .square = CHESS_SQUARE_F1, .file = CHESS_FILE_F },
		{ .square = CHESS_SQUARE_G1, .file = CHESS_FILE_G },
		{ .square = CHESS_SQUARE_H1, .file = CHESS_FILE_H },

		{ .square = CHESS_SQUARE_A2, .file = CHESS_FILE_A },
		{ .square = CHESS_SQUARE_B2, .file = CHESS_FILE_B },
		{ .square = CHESS_SQUARE_C2, .file = CHESS_FILE_C },
		{ .square = CHESS_SQUARE_D2, .file = CHESS_FILE_D },
		{ .square = CHESS_SQUARE_E2, .file = CHESS_FILE_E },
		{ .square = CHESS_SQUARE_F2, .file = CHESS_FILE_F },
		{ .square = CHESS_SQUARE_G2, .file = CHESS_FILE_G },
		{ .square = CHESS_SQUARE_H2, .file = CHESS_FILE_H },

		{ .square = CHESS_SQUARE_A3, .file = CHESS_FILE_A },
		{ .square = CHESS_SQUARE_B3, .file = CHESS_FILE_B },
		{ .square = CHESS_SQUARE_C3, .file = CHESS_FILE_C },
		{ .square = CHESS_SQUARE_D3, .file = CHESS_FILE_D },
		{ .square = CHESS_SQUARE_E3, .file = CHESS_FILE_E },
		{ .square = CHESS_SQUARE_F3, .file = CHESS_FILE_F },
		{ .square = CHESS_SQUARE_G3, .file = CHESS_FILE_G },
		{ .square = CHESS_SQUARE_H3, .file = CHESS_FILE_H },

		{ .square = CHESS_SQUARE_A4, .file = CHESS_FILE_A },
		{ .square = CHESS_SQUARE_B4, .file = CHESS_FILE_B },
		{ .square = CHESS_SQUARE_C4, .file = CHESS_FILE_C },
		{ .square = CHESS_SQUARE_D4, .file = CHESS_FILE_D },
		{ .square = CHESS_SQUARE_E4, .file = CHESS_FILE_E },
		{ .square = CHESS_SQUARE_F4, .file = CHESS_FILE_F },
		{ .square = CHESS_SQUARE_G4, .file = CHESS_FILE_G },
		{ .square = CHESS_SQUARE_H4, .file = CHESS_FILE_H },

		{ .square = CHESS_SQUARE_A5, .file = CHESS_FILE_A },
		{ .square = CHESS_SQUARE_B5, .file = CHESS_FILE_B },
		{ .square = CHESS_SQUARE_C5, .file = CHESS_FILE_C },
		{ .square = CHESS_SQUARE_D5, .file = CHESS_FILE_D },
		{ .square = CHESS_SQUARE_E5, .file = CHESS_FILE_E },
		{ .square = CHESS_SQUARE_F5, .file = CHESS_FILE_F },
		{ .square = CHESS_SQUARE_G5, .file = CHESS_FILE_G },
		{ .square = CHESS_SQUARE_H5, .file = CHESS_FILE_H },

		{ .square = CHESS_SQUARE_A6, .file = CHESS_FILE_A },
		{ .square = CHESS_SQUARE_B6, .file = CHESS_FILE_B },
		{ .square = CHESS_SQUARE_C6, .file = CHESS_FILE_C },
		{ .square = CHESS_SQUARE_D6, .file = CHESS_FILE_D },
		{ .square = CHESS_SQUARE_E6, .file = CHESS_FILE_E },
		{ .square = CHESS_SQUARE_F6, .file = CHESS_FILE_F },
		{ .square = CHESS_SQUARE_G6, .file = CHESS_FILE_G },
		{ .square = CHESS_SQUARE_H6, .file = CHESS_FILE_H },

		{ .square = CHESS_SQUARE_A7, .file = CHESS_FILE_A },
		{ .square = CHESS_SQUARE_B7, .file = CHESS_FILE_B },
		{ .square = CHESS_SQUARE_C7, .file = CHESS_FILE_C },
		{ .square = CHESS_SQUARE_D7, .file = CHESS_FILE_D },
		{ .square = CHESS_SQUARE_E7, .file = CHESS_FILE_E },
		{ .square = CHESS_SQUARE_F7, .file = CHESS_FILE_F },
		{ .square = CHESS_SQUARE_G7, .file = CHESS_FILE_G },
		{ .square = CHESS_SQUARE_H7, .file = CHESS_FILE_H },

		{ .square = CHESS_SQUARE_A8, .file = CHESS_FILE_A },
		{ .square = CHESS_SQUARE_B8, .file = CHESS_FILE_B },
		{ .square = CHESS_SQUARE_C8, .file = CHESS_FILE_C },
		{ .square = CHESS_SQUARE_D8, .file = CHESS_FILE_D },
		{ .square = CHESS_SQUARE_E8, .file = CHESS_FILE_E },
		{ .square = CHESS_SQUARE_F8, .file = CHESS_FILE_F },
		{ .square = CHESS_SQUARE_G8, .file = CHESS_FILE_G },
		{ .square = CHESS_SQUARE_H8, .file = CHESS_FILE_H },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessFile file = chess_square_file(test_cases[i].square);
		assert_int_equal(file, test_cases[i].file);
	}
}
static void test_chess_square_rank(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessSquare square;
		ChessRank rank;
	} test_cases[] = {
		{ .square = CHESS_SQUARE_A1, .rank = CHESS_RANK_1 },
		{ .square = CHESS_SQUARE_B1, .rank = CHESS_RANK_1 },
		{ .square = CHESS_SQUARE_C1, .rank = CHESS_RANK_1 },
		{ .square = CHESS_SQUARE_D1, .rank = CHESS_RANK_1 },
		{ .square = CHESS_SQUARE_E1, .rank = CHESS_RANK_1 },
		{ .square = CHESS_SQUARE_F1, .rank = CHESS_RANK_1 },
		{ .square = CHESS_SQUARE_G1, .rank = CHESS_RANK_1 },
		{ .square = CHESS_SQUARE_H1, .rank = CHESS_RANK_1 },

		{ .square = CHESS_SQUARE_A2, .rank = CHESS_RANK_2 },
		{ .square = CHESS_SQUARE_B2, .rank = CHESS_RANK_2 },
		{ .square = CHESS_SQUARE_C2, .rank = CHESS_RANK_2 },
		{ .square = CHESS_SQUARE_D2, .rank = CHESS_RANK_2 },
		{ .square = CHESS_SQUARE_E2, .rank = CHESS_RANK_2 },
		{ .square = CHESS_SQUARE_F2, .rank = CHESS_RANK_2 },
		{ .square = CHESS_SQUARE_G2, .rank = CHESS_RANK_2 },
		{ .square = CHESS_SQUARE_H2, .rank = CHESS_RANK_2 },

		{ .square = CHESS_SQUARE_A3, .rank = CHESS_RANK_3 },
		{ .square = CHESS_SQUARE_B3, .rank = CHESS_RANK_3 },
		{ .square = CHESS_SQUARE_C3, .rank = CHESS_RANK_3 },
		{ .square = CHESS_SQUARE_D3, .rank = CHESS_RANK_3 },
		{ .square = CHESS_SQUARE_E3, .rank = CHESS_RANK_3 },
		{ .square = CHESS_SQUARE_F3, .rank = CHESS_RANK_3 },
		{ .square = CHESS_SQUARE_G3, .rank = CHESS_RANK_3 },
		{ .square = CHESS_SQUARE_H3, .rank = CHESS_RANK_3 },

		{ .square = CHESS_SQUARE_A4, .rank = CHESS_RANK_4 },
		{ .square = CHESS_SQUARE_B4, .rank = CHESS_RANK_4 },
		{ .square = CHESS_SQUARE_C4, .rank = CHESS_RANK_4 },
		{ .square = CHESS_SQUARE_D4, .rank = CHESS_RANK_4 },
		{ .square = CHESS_SQUARE_E4, .rank = CHESS_RANK_4 },
		{ .square = CHESS_SQUARE_F4, .rank = CHESS_RANK_4 },
		{ .square = CHESS_SQUARE_G4, .rank = CHESS_RANK_4 },
		{ .square = CHESS_SQUARE_H4, .rank = CHESS_RANK_4 },

		{ .square = CHESS_SQUARE_A5, .rank = CHESS_RANK_5 },
		{ .square = CHESS_SQUARE_B5, .rank = CHESS_RANK_5 },
		{ .square = CHESS_SQUARE_C5, .rank = CHESS_RANK_5 },
		{ .square = CHESS_SQUARE_D5, .rank = CHESS_RANK_5 },
		{ .square = CHESS_SQUARE_E5, .rank = CHESS_RANK_5 },
		{ .square = CHESS_SQUARE_F5, .rank = CHESS_RANK_5 },
		{ .square = CHESS_SQUARE_G5, .rank = CHESS_RANK_5 },
		{ .square = CHESS_SQUARE_H5, .rank = CHESS_RANK_5 },

		{ .square = CHESS_SQUARE_A6, .rank = CHESS_RANK_6 },
		{ .square = CHESS_SQUARE_B6, .rank = CHESS_RANK_6 },
		{ .square = CHESS_SQUARE_C6, .rank = CHESS_RANK_6 },
		{ .square = CHESS_SQUARE_D6, .rank = CHESS_RANK_6 },
		{ .square = CHESS_SQUARE_E6, .rank = CHESS_RANK_6 },
		{ .square = CHESS_SQUARE_F6, .rank = CHESS_RANK_6 },
		{ .square = CHESS_SQUARE_G6, .rank = CHESS_RANK_6 },
		{ .square = CHESS_SQUARE_H6, .rank = CHESS_RANK_6 },

		{ .square = CHESS_SQUARE_A7, .rank = CHESS_RANK_7 },
		{ .square = CHESS_SQUARE_B7, .rank = CHESS_RANK_7 },
		{ .square = CHESS_SQUARE_C7, .rank = CHESS_RANK_7 },
		{ .square = CHESS_SQUARE_D7, .rank = CHESS_RANK_7 },
		{ .square = CHESS_SQUARE_E7, .rank = CHESS_RANK_7 },
		{ .square = CHESS_SQUARE_F7, .rank = CHESS_RANK_7 },
		{ .square = CHESS_SQUARE_G7, .rank = CHESS_RANK_7 },
		{ .square = CHESS_SQUARE_H7, .rank = CHESS_RANK_7 },

		{ .square = CHESS_SQUARE_A8, .rank = CHESS_RANK_8 },
		{ .square = CHESS_SQUARE_B8, .rank = CHESS_RANK_8 },
		{ .square = CHESS_SQUARE_C8, .rank = CHESS_RANK_8 },
		{ .square = CHESS_SQUARE_D8, .rank = CHESS_RANK_8 },
		{ .square = CHESS_SQUARE_E8, .rank = CHESS_RANK_8 },
		{ .square = CHESS_SQUARE_F8, .rank = CHESS_RANK_8 },
		{ .square = CHESS_SQUARE_G8, .rank = CHESS_RANK_8 },
		{ .square = CHESS_SQUARE_H8, .rank = CHESS_RANK_8 },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessRank rank = chess_square_rank(test_cases[i].square);
		assert_int_equal(rank, test_cases[i].rank);
	}
}
static void test_chess_square_is_valid(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessSquare square;
		bool is_valid;
	} test_cases[] = {
		{ .square = CHESS_SQUARE_NONE, .is_valid = false },

		{ .square = CHESS_SQUARE_A1, .is_valid = true },
		{ .square = CHESS_SQUARE_B1, .is_valid = true },
		{ .square = CHESS_SQUARE_C1, .is_valid = true },
		{ .square = CHESS_SQUARE_D1, .is_valid = true },
		{ .square = CHESS_SQUARE_E1, .is_valid = true },
		{ .square = CHESS_SQUARE_F1, .is_valid = true },
		{ .square = CHESS_SQUARE_G1, .is_valid = true },
		{ .square = CHESS_SQUARE_H1, .is_valid = true },

		{ .square = CHESS_SQUARE_A2, .is_valid = true },
		{ .square = CHESS_SQUARE_B2, .is_valid = true },
		{ .square = CHESS_SQUARE_C2, .is_valid = true },
		{ .square = CHESS_SQUARE_D2, .is_valid = true },
		{ .square = CHESS_SQUARE_E2, .is_valid = true },
		{ .square = CHESS_SQUARE_F2, .is_valid = true },
		{ .square = CHESS_SQUARE_G2, .is_valid = true },
		{ .square = CHESS_SQUARE_H2, .is_valid = true },

		{ .square = CHESS_SQUARE_A3, .is_valid = true },
		{ .square = CHESS_SQUARE_B3, .is_valid = true },
		{ .square = CHESS_SQUARE_C3, .is_valid = true },
		{ .square = CHESS_SQUARE_D3, .is_valid = true },
		{ .square = CHESS_SQUARE_E3, .is_valid = true },
		{ .square = CHESS_SQUARE_F3, .is_valid = true },
		{ .square = CHESS_SQUARE_G3, .is_valid = true },
		{ .square = CHESS_SQUARE_H3, .is_valid = true },

		{ .square = CHESS_SQUARE_A4, .is_valid = true },
		{ .square = CHESS_SQUARE_B4, .is_valid = true },
		{ .square = CHESS_SQUARE_C4, .is_valid = true },
		{ .square = CHESS_SQUARE_D4, .is_valid = true },
		{ .square = CHESS_SQUARE_E4, .is_valid = true },
		{ .square = CHESS_SQUARE_F4, .is_valid = true },
		{ .square = CHESS_SQUARE_G4, .is_valid = true },
		{ .square = CHESS_SQUARE_H4, .is_valid = true },

		{ .square = CHESS_SQUARE_A5, .is_valid = true },
		{ .square = CHESS_SQUARE_B5, .is_valid = true },
		{ .square = CHESS_SQUARE_C5, .is_valid = true },
		{ .square = CHESS_SQUARE_D5, .is_valid = true },
		{ .square = CHESS_SQUARE_E5, .is_valid = true },
		{ .square = CHESS_SQUARE_F5, .is_valid = true },
		{ .square = CHESS_SQUARE_G5, .is_valid = true },
		{ .square = CHESS_SQUARE_H5, .is_valid = true },

		{ .square = CHESS_SQUARE_A6, .is_valid = true },
		{ .square = CHESS_SQUARE_B6, .is_valid = true },
		{ .square = CHESS_SQUARE_C6, .is_valid = true },
		{ .square = CHESS_SQUARE_D6, .is_valid = true },
		{ .square = CHESS_SQUARE_E6, .is_valid = true },
		{ .square = CHESS_SQUARE_F6, .is_valid = true },
		{ .square = CHESS_SQUARE_G6, .is_valid = true },
		{ .square = CHESS_SQUARE_H6, .is_valid = true },

		{ .square = CHESS_SQUARE_A7, .is_valid = true },
		{ .square = CHESS_SQUARE_B7, .is_valid = true },
		{ .square = CHESS_SQUARE_C7, .is_valid = true },
		{ .square = CHESS_SQUARE_D7, .is_valid = true },
		{ .square = CHESS_SQUARE_E7, .is_valid = true },
		{ .square = CHESS_SQUARE_F7, .is_valid = true },
		{ .square = CHESS_SQUARE_G7, .is_valid = true },
		{ .square = CHESS_SQUARE_H7, .is_valid = true },

		{ .square = CHESS_SQUARE_A8, .is_valid = true },
		{ .square = CHESS_SQUARE_B8, .is_valid = true },
		{ .square = CHESS_SQUARE_C8, .is_valid = true },
		{ .square = CHESS_SQUARE_D8, .is_valid = true },
		{ .square = CHESS_SQUARE_E8, .is_valid = true },
		{ .square = CHESS_SQUARE_F8, .is_valid = true },
		{ .square = CHESS_SQUARE_G8, .is_valid = true },
		{ .square = CHESS_SQUARE_H8, .is_valid = true },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		bool is_valid = chess_square_is_valid(test_cases[i].square);
		if (test_cases[i].is_valid) {
			assert_true(is_valid);
		} else {
			assert_false(is_valid);
		}
	}
}
static void test_chess_square_from_algebraic(void **state) {
	(void)state;

	static const struct {
		const char *string;
		ChessSquare square;
		size_t read;
	} test_cases[] = {
		{ .string = "a1", .square = CHESS_SQUARE_A1, .read = 2 },
		{ .string = "b1", .square = CHESS_SQUARE_B1, .read = 2 },
		{ .string = "c1", .square = CHESS_SQUARE_C1, .read = 2 },
		{ .string = "d1", .square = CHESS_SQUARE_D1, .read = 2 },
		{ .string = "e1", .square = CHESS_SQUARE_E1, .read = 2 },
		{ .string = "f1", .square = CHESS_SQUARE_F1, .read = 2 },
		{ .string = "g1", .square = CHESS_SQUARE_G1, .read = 2 },
		{ .string = "h1", .square = CHESS_SQUARE_H1, .read = 2 },

		{ .string = "a2", .square = CHESS_SQUARE_A2, .read = 2 },
		{ .string = "b2", .square = CHESS_SQUARE_B2, .read = 2 },
		{ .string = "c2", .square = CHESS_SQUARE_C2, .read = 2 },
		{ .string = "d2", .square = CHESS_SQUARE_D2, .read = 2 },
		{ .string = "e2", .square = CHESS_SQUARE_E2, .read = 2 },
		{ .string = "f2", .square = CHESS_SQUARE_F2, .read = 2 },
		{ .string = "g2", .square = CHESS_SQUARE_G2, .read = 2 },
		{ .string = "h2", .square = CHESS_SQUARE_H2, .read = 2 },

		{ .string = "a3", .square = CHESS_SQUARE_A3, .read = 2 },
		{ .string = "b3", .square = CHESS_SQUARE_B3, .read = 2 },
		{ .string = "c3", .square = CHESS_SQUARE_C3, .read = 2 },
		{ .string = "d3", .square = CHESS_SQUARE_D3, .read = 2 },
		{ .string = "e3", .square = CHESS_SQUARE_E3, .read = 2 },
		{ .string = "f3", .square = CHESS_SQUARE_F3, .read = 2 },
		{ .string = "g3", .square = CHESS_SQUARE_G3, .read = 2 },
		{ .string = "h3", .square = CHESS_SQUARE_H3, .read = 2 },

		{ .string = "a4", .square = CHESS_SQUARE_A4, .read = 2 },
		{ .string = "b4", .square = CHESS_SQUARE_B4, .read = 2 },
		{ .string = "c4", .square = CHESS_SQUARE_C4, .read = 2 },
		{ .string = "d4", .square = CHESS_SQUARE_D4, .read = 2 },
		{ .string = "e4", .square = CHESS_SQUARE_E4, .read = 2 },
		{ .string = "f4", .square = CHESS_SQUARE_F4, .read = 2 },
		{ .string = "g4", .square = CHESS_SQUARE_G4, .read = 2 },
		{ .string = "h4", .square = CHESS_SQUARE_H4, .read = 2 },

		{ .string = "a5", .square = CHESS_SQUARE_A5, .read = 2 },
		{ .string = "b5", .square = CHESS_SQUARE_B5, .read = 2 },
		{ .string = "c5", .square = CHESS_SQUARE_C5, .read = 2 },
		{ .string = "d5", .square = CHESS_SQUARE_D5, .read = 2 },
		{ .string = "e5", .square = CHESS_SQUARE_E5, .read = 2 },
		{ .string = "f5", .square = CHESS_SQUARE_F5, .read = 2 },
		{ .string = "g5", .square = CHESS_SQUARE_G5, .read = 2 },
		{ .string = "h5", .square = CHESS_SQUARE_H5, .read = 2 },

		{ .string = "a6", .square = CHESS_SQUARE_A6, .read = 2 },
		{ .string = "b6", .square = CHESS_SQUARE_B6, .read = 2 },
		{ .string = "c6", .square = CHESS_SQUARE_C6, .read = 2 },
		{ .string = "d6", .square = CHESS_SQUARE_D6, .read = 2 },
		{ .string = "e6", .square = CHESS_SQUARE_E6, .read = 2 },
		{ .string = "f6", .square = CHESS_SQUARE_F6, .read = 2 },
		{ .string = "g6", .square = CHESS_SQUARE_G6, .read = 2 },
		{ .string = "h6", .square = CHESS_SQUARE_H6, .read = 2 },

		{ .string = "a7", .square = CHESS_SQUARE_A7, .read = 2 },
		{ .string = "b7", .square = CHESS_SQUARE_B7, .read = 2 },
		{ .string = "c7", .square = CHESS_SQUARE_C7, .read = 2 },
		{ .string = "d7", .square = CHESS_SQUARE_D7, .read = 2 },
		{ .string = "e7", .square = CHESS_SQUARE_E7, .read = 2 },
		{ .string = "f7", .square = CHESS_SQUARE_F7, .read = 2 },
		{ .string = "g7", .square = CHESS_SQUARE_G7, .read = 2 },
		{ .string = "h7", .square = CHESS_SQUARE_H7, .read = 2 },

		{ .string = "a8", .square = CHESS_SQUARE_A8, .read = 2 },
		{ .string = "b8", .square = CHESS_SQUARE_B8, .read = 2 },
		{ .string = "c8", .square = CHESS_SQUARE_C8, .read = 2 },
		{ .string = "d8", .square = CHESS_SQUARE_D8, .read = 2 },
		{ .string = "e8", .square = CHESS_SQUARE_E8, .read = 2 },
		{ .string = "f8", .square = CHESS_SQUARE_F8, .read = 2 },
		{ .string = "g8", .square = CHESS_SQUARE_G8, .read = 2 },
		{ .string = "h8", .square = CHESS_SQUARE_H8, .read = 2 },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		ChessSquare square = CHESS_SQUARE_NONE;
		size_t read        = chess_square_from_algebraic(&square, test_cases[i].string);
		assert_int_equal(square, test_cases[i].square);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_square_to_algebraic(void **state) {
	(void)state;

	static const struct {
		ChessSquare square;
		const char *string;
		size_t written;
	} test_cases[] = {
		{ .square = CHESS_SQUARE_A1, .string = "a1", .written = 2 },
		{ .square = CHESS_SQUARE_B1, .string = "b1", .written = 2 },
		{ .square = CHESS_SQUARE_C1, .string = "c1", .written = 2 },
		{ .square = CHESS_SQUARE_D1, .string = "d1", .written = 2 },
		{ .square = CHESS_SQUARE_E1, .string = "e1", .written = 2 },
		{ .square = CHESS_SQUARE_F1, .string = "f1", .written = 2 },
		{ .square = CHESS_SQUARE_G1, .string = "g1", .written = 2 },
		{ .square = CHESS_SQUARE_H1, .string = "h1", .written = 2 },

		{ .square = CHESS_SQUARE_A2, .string = "a2", .written = 2 },
		{ .square = CHESS_SQUARE_B2, .string = "b2", .written = 2 },
		{ .square = CHESS_SQUARE_C2, .string = "c2", .written = 2 },
		{ .square = CHESS_SQUARE_D2, .string = "d2", .written = 2 },
		{ .square = CHESS_SQUARE_E2, .string = "e2", .written = 2 },
		{ .square = CHESS_SQUARE_F2, .string = "f2", .written = 2 },
		{ .square = CHESS_SQUARE_G2, .string = "g2", .written = 2 },
		{ .square = CHESS_SQUARE_H2, .string = "h2", .written = 2 },

		{ .square = CHESS_SQUARE_A3, .string = "a3", .written = 2 },
		{ .square = CHESS_SQUARE_B3, .string = "b3", .written = 2 },
		{ .square = CHESS_SQUARE_C3, .string = "c3", .written = 2 },
		{ .square = CHESS_SQUARE_D3, .string = "d3", .written = 2 },
		{ .square = CHESS_SQUARE_E3, .string = "e3", .written = 2 },
		{ .square = CHESS_SQUARE_F3, .string = "f3", .written = 2 },
		{ .square = CHESS_SQUARE_G3, .string = "g3", .written = 2 },
		{ .square = CHESS_SQUARE_H3, .string = "h3", .written = 2 },

		{ .square = CHESS_SQUARE_A4, .string = "a4", .written = 2 },
		{ .square = CHESS_SQUARE_B4, .string = "b4", .written = 2 },
		{ .square = CHESS_SQUARE_C4, .string = "c4", .written = 2 },
		{ .square = CHESS_SQUARE_D4, .string = "d4", .written = 2 },
		{ .square = CHESS_SQUARE_E4, .string = "e4", .written = 2 },
		{ .square = CHESS_SQUARE_F4, .string = "f4", .written = 2 },
		{ .square = CHESS_SQUARE_G4, .string = "g4", .written = 2 },
		{ .square = CHESS_SQUARE_H4, .string = "h4", .written = 2 },

		{ .square = CHESS_SQUARE_A5, .string = "a5", .written = 2 },
		{ .square = CHESS_SQUARE_B5, .string = "b5", .written = 2 },
		{ .square = CHESS_SQUARE_C5, .string = "c5", .written = 2 },
		{ .square = CHESS_SQUARE_D5, .string = "d5", .written = 2 },
		{ .square = CHESS_SQUARE_E5, .string = "e5", .written = 2 },
		{ .square = CHESS_SQUARE_F5, .string = "f5", .written = 2 },
		{ .square = CHESS_SQUARE_G5, .string = "g5", .written = 2 },
		{ .square = CHESS_SQUARE_H5, .string = "h5", .written = 2 },

		{ .square = CHESS_SQUARE_A6, .string = "a6", .written = 2 },
		{ .square = CHESS_SQUARE_B6, .string = "b6", .written = 2 },
		{ .square = CHESS_SQUARE_C6, .string = "c6", .written = 2 },
		{ .square = CHESS_SQUARE_D6, .string = "d6", .written = 2 },
		{ .square = CHESS_SQUARE_E6, .string = "e6", .written = 2 },
		{ .square = CHESS_SQUARE_F6, .string = "f6", .written = 2 },
		{ .square = CHESS_SQUARE_G6, .string = "g6", .written = 2 },
		{ .square = CHESS_SQUARE_H6, .string = "h6", .written = 2 },

		{ .square = CHESS_SQUARE_A7, .string = "a7", .written = 2 },
		{ .square = CHESS_SQUARE_B7, .string = "b7", .written = 2 },
		{ .square = CHESS_SQUARE_C7, .string = "c7", .written = 2 },
		{ .square = CHESS_SQUARE_D7, .string = "d7", .written = 2 },
		{ .square = CHESS_SQUARE_E7, .string = "e7", .written = 2 },
		{ .square = CHESS_SQUARE_F7, .string = "f7", .written = 2 },
		{ .square = CHESS_SQUARE_G7, .string = "g7", .written = 2 },
		{ .square = CHESS_SQUARE_H7, .string = "h7", .written = 2 },

		{ .square = CHESS_SQUARE_A8, .string = "a8", .written = 2 },
		{ .square = CHESS_SQUARE_B8, .string = "b8", .written = 2 },
		{ .square = CHESS_SQUARE_C8, .string = "c8", .written = 2 },
		{ .square = CHESS_SQUARE_D8, .string = "d8", .written = 2 },
		{ .square = CHESS_SQUARE_E8, .string = "e8", .written = 2 },
		{ .square = CHESS_SQUARE_F8, .string = "f8", .written = 2 },
		{ .square = CHESS_SQUARE_G8, .string = "g8", .written = 2 },
		{ .square = CHESS_SQUARE_H8, .string = "h8", .written = 2 },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		char string[3] = { 0 };
		size_t written = chess_square_to_algebraic(test_cases[i].square, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

static void test_chess_move_is_valid(void **state) {
	(void)state;

	static CHESS_CONSTEXPR struct {
		ChessMove move;
		bool is_valid;
	} test_cases[] = {
		{
		    .move = (ChessMove){
		        .from           = CHESS_SQUARE_NONE,
		        .to             = CHESS_SQUARE_A2,
		        .promotion_type = CHESS_PIECE_TYPE_NONE,
		    },
		    .is_valid = false,
		},
		{
		    .move = (ChessMove){
		        .from           = CHESS_SQUARE_A2,
		        .to             = CHESS_SQUARE_NONE,
		        .promotion_type = CHESS_PIECE_TYPE_NONE,
		    },
		    .is_valid = false,
		},
		{
		    .move = (ChessMove){
		        .from           = CHESS_SQUARE_A2,
		        .to             = CHESS_SQUARE_A1,
		        .promotion_type = CHESS_PIECE_TYPE_KING,
		    },
		    .is_valid = false,
		},

		{
		    .move = (ChessMove){
		        .from           = CHESS_SQUARE_A2,
		        .to             = CHESS_SQUARE_A1,
		        .promotion_type = CHESS_PIECE_TYPE_KNIGHT,
		    },
		    .is_valid = true,
		},
		{
		    .move = (ChessMove){
		        .from           = CHESS_SQUARE_B1,
		        .to             = CHESS_SQUARE_C3,
		        .promotion_type = CHESS_PIECE_TYPE_NONE,
		    },
		    .is_valid = true,
		},
		{
		    .move = (ChessMove){
		        .from           = CHESS_SQUARE_E2,
		        .to             = CHESS_SQUARE_E4,
		        .promotion_type = CHESS_PIECE_TYPE_NONE,
		    },
		    .is_valid = true,
		},
		{
		    .move = (ChessMove){
		        .from           = CHESS_SQUARE_C7,
		        .to             = CHESS_SQUARE_C8,
		        .promotion_type = CHESS_PIECE_TYPE_QUEEN,
		    },
		    .is_valid = true,
		}
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		bool is_valid = chess_move_is_valid(test_cases[i].move);
		if (test_cases[i].is_valid) {
			assert_true(is_valid);
		} else {
			assert_false(is_valid);
		}
	}
}

static void test_chess_preft(void **state) {
	(void)state;

	static CHESS_CONSTEXPR unsigned long expected[] = {
		1,
		20,
		400,
		8902,
		197281
	};

	for (unsigned int depth = 0; depth < ARRAY_LENGTH(expected); depth++) {
		assert_int_equal(chess_perft(depth), expected[depth]);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_color_is_valid),
		cmocka_unit_test(test_chess_color_opposite),

		cmocka_unit_test(test_chess_piece_type_is_valid),
		cmocka_unit_test(test_chess_piece_type_from_algebraic),
		cmocka_unit_test(test_chess_piece_type_to_algebraic),

		cmocka_unit_test(test_chess_piece_new),
		cmocka_unit_test(test_chess_piece_is_valid),
		cmocka_unit_test(test_chess_piece_color),
		cmocka_unit_test(test_chess_piece_type),
		cmocka_unit_test(test_chess_piece_from_algebraic),
		cmocka_unit_test(test_chess_piece_to_algebraic),

		cmocka_unit_test(test_chess_file_is_valid),
		cmocka_unit_test(test_chess_file_from_algebraic),
		cmocka_unit_test(test_chess_file_to_algebraic),

		cmocka_unit_test(test_chess_rank_is_valid),
		cmocka_unit_test(test_chess_rank_from_algebraic),
		cmocka_unit_test(test_chess_rank_to_algebraic),

		cmocka_unit_test(test_chess_square_new),
		cmocka_unit_test(test_chess_square_file),
		cmocka_unit_test(test_chess_square_rank),
		cmocka_unit_test(test_chess_square_is_valid),
		cmocka_unit_test(test_chess_square_from_algebraic),
		cmocka_unit_test(test_chess_square_to_algebraic),

		cmocka_unit_test(test_chess_move_is_valid),

		cmocka_unit_test(test_chess_preft),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
