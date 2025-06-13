#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/square.h>

static void test_chess_square_new(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessFile file;
		ChessRank rank;
		ChessSquare square;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
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

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessSquare square = chess_square_new(test_cases[i].file, test_cases[i].rank);
		assert_int_equal(square, test_cases[i].square);
	}
}
static void test_chess_square_file(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessSquare square;
		ChessFile file;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
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

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessFile file = chess_square_file(test_cases[i].square);
		assert_int_equal(file, test_cases[i].file);
	}
}
static void test_chess_square_rank(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessSquare square;
		ChessRank rank;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
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

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessRank rank = chess_square_rank(test_cases[i].square);
		assert_int_equal(rank, test_cases[i].rank);
	}
}
static void test_chess_square_is_valid(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessSquare square;
		bool is_valid;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
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

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
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

	typedef struct TestCase {
		const char *string;
		ChessSquare square;
		size_t read;
	} TestCase;

	static const TestCase test_cases[] = {
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

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessSquare square = CHESS_SQUARE_NONE;
		size_t read        = chess_square_from_algebraic(&square, test_cases[i].string);
		assert_int_equal(square, test_cases[i].square);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_square_to_algebraic(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessSquare square;
		const char *string;
		size_t written;
	} TestCase;

	static const TestCase test_cases[] = {
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

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		char string[3] = { 0 };
		size_t written = chess_square_to_algebraic(test_cases[i].square, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_square_new),
		cmocka_unit_test(test_chess_square_file),
		cmocka_unit_test(test_chess_square_rank),
		cmocka_unit_test(test_chess_square_is_valid),
		cmocka_unit_test(test_chess_square_from_algebraic),
		cmocka_unit_test(test_chess_square_to_algebraic),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
