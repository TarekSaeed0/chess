#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/move.h>

static void test_chess_move_is_valid(void **state) {
	(void)state;

	typedef struct TestCase {
		ChessMove move;
		bool is_valid;
	} TestCase;

	static CHESS_CONSTEXPR TestCase test_cases[] = {
		{
		    .move = {
		        .from                       = CHESS_SQUARE_NONE,
		        .to                         = CHESS_SQUARE_A2,
		        .promotion_type             = CHESS_PIECE_TYPE_NONE,
		        .captured_piece             = CHESS_PIECE_NONE,
		        .previous_castling_rights   = CHESS_CASTLING_RIGHTS_NONE,
		        .previous_en_passant_square = CHESS_SQUARE_NONE,
		        .previous_half_move_clock   = 0,
		    },
		    .is_valid = false,
		},
		{
		    .move = {
		        .from                       = CHESS_SQUARE_A2,
		        .to                         = CHESS_SQUARE_NONE,
		        .promotion_type             = CHESS_PIECE_TYPE_NONE,
		        .captured_piece             = CHESS_PIECE_NONE,
		        .previous_castling_rights   = CHESS_CASTLING_RIGHTS_NONE,
		        .previous_en_passant_square = CHESS_SQUARE_NONE,
		        .previous_half_move_clock   = 0,
		    },
		    .is_valid = false,
		},
		{
		    .move = {
		        .from                       = CHESS_SQUARE_A2,
		        .to                         = CHESS_SQUARE_A1,
		        .promotion_type             = CHESS_PIECE_TYPE_KING,
		        .captured_piece             = CHESS_PIECE_NONE,
		        .previous_castling_rights   = CHESS_CASTLING_RIGHTS_NONE,
		        .previous_en_passant_square = CHESS_SQUARE_NONE,
		        .previous_half_move_clock   = 0,
		    },
		    .is_valid = false,
		},

		{
		    .move = {
		        .from                       = CHESS_SQUARE_A2,
		        .to                         = CHESS_SQUARE_A1,
		        .promotion_type             = CHESS_PIECE_TYPE_KNIGHT,
		        .captured_piece             = CHESS_PIECE_NONE,
		        .previous_castling_rights   = CHESS_CASTLING_RIGHTS_NONE,
		        .previous_en_passant_square = CHESS_SQUARE_NONE,
		        .previous_half_move_clock   = 0,
		    },
		    .is_valid = true,
		},
		{
		    .move = {
		        .from                       = CHESS_SQUARE_B1,
		        .to                         = CHESS_SQUARE_C3,
		        .promotion_type             = CHESS_PIECE_TYPE_NONE,
		        .captured_piece             = CHESS_PIECE_NONE,
		        .previous_castling_rights   = CHESS_CASTLING_RIGHTS_NONE,
		        .previous_en_passant_square = CHESS_SQUARE_NONE,
		        .previous_half_move_clock   = 0,
		    },
		    .is_valid = true,
		},
		{
		    .move = {
		        .from                       = CHESS_SQUARE_E2,
		        .to                         = CHESS_SQUARE_E4,
		        .promotion_type             = CHESS_PIECE_TYPE_NONE,
		        .captured_piece             = CHESS_PIECE_NONE,
		        .previous_castling_rights   = CHESS_CASTLING_RIGHTS_NONE,
		        .previous_en_passant_square = CHESS_SQUARE_NONE,
		        .previous_half_move_clock   = 0,
		    },
		    .is_valid = true,
		},
		{
		    .move = {
		        .from                       = CHESS_SQUARE_C7,
		        .to                         = CHESS_SQUARE_C8,
		        .promotion_type             = CHESS_PIECE_TYPE_QUEEN,
		        .captured_piece             = CHESS_PIECE_NONE,
		        .previous_castling_rights   = CHESS_CASTLING_RIGHTS_NONE,
		        .previous_en_passant_square = CHESS_SQUARE_NONE,
		        .previous_half_move_clock   = 0,
		    },
		    .is_valid = true,
		}
	};

	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		bool is_valid = chess_move_is_valid(test_cases[i].move);
		if (test_cases[i].is_valid) {
			assert_true(is_valid);
		} else {
			assert_false(is_valid);
		}
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_move_is_valid),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
