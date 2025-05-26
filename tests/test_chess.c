#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

static void test_chess_color_is_valid(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_color color;
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

	static constexpr struct {
		enum chess_color color;
		enum chess_color opposite;
	} test_cases[] = {
		{ .color = CHESS_COLOR_WHITE, .opposite = CHESS_COLOR_BLACK },
		{ .color = CHESS_COLOR_BLACK, .opposite = CHESS_COLOR_WHITE },
	};

	for (size_t i = 0; i < ARRAY_LENGTH(test_cases); i++) {
		enum chess_color opposite = chess_color_opposite(test_cases[i].color);
		assert_int_equal(opposite, test_cases[i].opposite);
	}
}

static void test_chess_piece_type_is_valid(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_piece_type type;
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

static void test_chess_piece_is_valid(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_piece piece;
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

static void test_chess_file_is_valid(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_file file;
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

static void test_chess_rank_is_valid(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_rank rank;
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

static void test_chess_square_new(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_file file;
		enum chess_rank rank;
		enum chess_square square;
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
		enum chess_square square = chess_square_new(test_cases[i].file, test_cases[i].rank);
		assert_int_equal(square, test_cases[i].square);
	}
}
static void test_chess_square_file(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_square square;
		enum chess_file file;
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
		enum chess_file file = chess_square_file(test_cases[i].square);
		assert_int_equal(file, test_cases[i].file);
	}
}
static void test_chess_square_rank(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_square square;
		enum chess_rank rank;
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
		enum chess_rank rank = chess_square_rank(test_cases[i].square);
		assert_int_equal(rank, test_cases[i].rank);
	}
}
static void test_chess_square_is_valid(void **state) {
	(void)state;

	static constexpr struct {
		enum chess_square square;
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

static void test_chess_preft(void **state) {
	(void)state;

	static constexpr unsigned long expected[] = {
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

		cmocka_unit_test(test_chess_piece_is_valid),

		cmocka_unit_test(test_chess_file_is_valid),

		cmocka_unit_test(test_chess_rank_is_valid),

		cmocka_unit_test(test_chess_square_new),
		cmocka_unit_test(test_chess_square_file),
		cmocka_unit_test(test_chess_square_rank),
		cmocka_unit_test(test_chess_square_is_valid),

		cmocka_unit_test(test_chess_preft),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
