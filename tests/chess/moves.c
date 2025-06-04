#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <chess/moves.h>

#include <chess/move_private.h>
#include <chess/position.h>

static unsigned long chess_moves_perft_recursive(const ChessPosition *position, unsigned int depth) {
	if (depth == 0) {
		return 1;
	}

	ChessMoves moves = chess_moves_generate(position);
	if (depth == 1) {
		return moves.count;
	}

	unsigned long count = 0;
	for (size_t i = 0; i < moves.count; i++) {
		ChessPosition position_after_move = *position;
		chess_move_do_unchecked(&position_after_move, moves.moves[i]);
		count += chess_moves_perft_recursive(&position_after_move, depth - 1);
	}

	return count;
}
static inline unsigned long chess_moves_perft(const char *fen, unsigned int depth) {
	ChessPosition position = chess_position_new();
	if (fen != CHESS_NULL && !chess_position_from_fen(&position, fen)) {
		return 0;
	}

	return chess_moves_perft_recursive(&position, depth);
}

static void test_chess_moves_preft(void **state) {
	(void)state;

	static const struct {
		const char *fen;
		unsigned long results[5];
		size_t results_count;
	} test_cases[] = {
		{
		    .fen     = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
		    .results = {
		        1,
		        20,
		        400,
		        8902,
		        197281,
		    },
		    .results_count = 5,
		},
		{
		    .fen     = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		    .results = {
		        1,
		        48,
		        2039,
		        97862,
		    },
		    .results_count = 4,
		},
		{
		    .fen     = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
		    .results = {
		        1,
		        14,
		        191,
		        2812,
		        43238,
		    },
		    .results_count = 5,
		},
		{
		    .fen     = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		    .results = {
		        1,
		        6,
		        264,
		        9467,
		        422333,
		    },
		    .results_count = 5,
		},
		{
		    .fen     = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
		    .results = {
		        1,
		        44,
		        1486,
		        62379,
		    },
		    .results_count = 4,
		},
		{
		    .fen     = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
		    .results = {
		        1,
		        46,
		        2079,
		        89890,
		    },
		    .results_count = 4,
		}
	};
	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(test_cases); i++) {
		for (unsigned int depth = 0; depth < test_cases[i].results_count; depth++) {
			assert_int_equal(chess_moves_perft(test_cases[i].fen, depth), test_cases[i].results[depth]);
		}
	}
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_chess_moves_preft),
	};

	return cmocka_run_group_tests(tests, CHESS_NULL, CHESS_NULL);
}
