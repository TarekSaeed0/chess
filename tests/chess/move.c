#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/move.h>

#include <chess/move_private.h>
#include <chess/moves.h>
#include <chess/position.h>

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
static void test_chess_move_is_legal_recursive(const ChessPosition *position, unsigned int depth) {
	if (depth == 0) {
		return;
	}

	ChessMoves moves = chess_moves_generate(position);
	for (size_t i = 0; i < moves.count; i++) {
		assert_true(chess_move_is_legal(position, moves.moves[i]));

		ChessPosition position_after_move = *position;
		chess_move_do_unchecked(&position_after_move, moves.moves[i]);
		test_chess_move_is_legal_recursive(&position_after_move, depth - 1);
	}
}
static void test_chess_move_is_legal(void **state) {
	(void)state;

	typedef struct TestCase {
		const char *fen;
	} TestCase;

	static const TestCase test_cases[] = {
		{
		    .fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
		},
		{
		    .fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		},
		{
		    .fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
		},
		{
		    .fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		},
		{
		    .fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
		},
		{
		    .fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
		}
	};
	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		ChessPosition position = chess_position_new();
		if (chess_position_from_fen(&position, test_cases[i].fen)) {
			test_chess_move_is_legal_recursive(&position, 4);
		}
		chess_position_drop(&position);
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_move_is_valid),
		cmocka_unit_test(test_chess_move_is_legal),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
