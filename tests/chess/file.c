#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/file.h>

static void test_chess_file_is_valid(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessFile file;
		bool is_valid;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
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

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
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

	typedef struct TestCase {
		const char *string;
		ChessFile file;
		size_t read;
	} TestCase;

	static const TestCase test_cases[] = {
		{ .string = "a", .file = CHESS_FILE_A, .read = 1 },
		{ .string = "b", .file = CHESS_FILE_B, .read = 1 },
		{ .string = "c", .file = CHESS_FILE_C, .read = 1 },
		{ .string = "d", .file = CHESS_FILE_D, .read = 1 },
		{ .string = "e", .file = CHESS_FILE_E, .read = 1 },
		{ .string = "f", .file = CHESS_FILE_F, .read = 1 },
		{ .string = "g", .file = CHESS_FILE_G, .read = 1 },
		{ .string = "h", .file = CHESS_FILE_H, .read = 1 },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessFile file = CHESS_FILE_NONE;
		size_t read    = chess_file_from_algebraic(&file, test_cases[i].string);
		assert_int_equal(file, test_cases[i].file);
		assert_int_equal(read, test_cases[i].read);
	}
}
static void test_chess_file_to_algebraic(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessFile file;
		const char *string;
		size_t written;
	} TestCase;

	static const TestCase test_cases[] = {
		{ .file = CHESS_FILE_A, .string = "a", .written = 1 },
		{ .file = CHESS_FILE_B, .string = "b", .written = 1 },
		{ .file = CHESS_FILE_C, .string = "c", .written = 1 },
		{ .file = CHESS_FILE_D, .string = "d", .written = 1 },
		{ .file = CHESS_FILE_E, .string = "e", .written = 1 },
		{ .file = CHESS_FILE_F, .string = "f", .written = 1 },
		{ .file = CHESS_FILE_G, .string = "g", .written = 1 },
		{ .file = CHESS_FILE_H, .string = "h", .written = 1 },
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		char string[2] = { 0 };
		size_t written = chess_file_to_algebraic(test_cases[i].file, string, sizeof(string));
		assert_string_equal(string, test_cases[i].string);
		assert_int_equal(written, test_cases[i].written);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_file_is_valid),
		cmocka_unit_test(test_chess_file_from_algebraic),
		cmocka_unit_test(test_chess_file_to_algebraic),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
