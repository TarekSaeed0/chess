#include <chess.h>

#include <chess/move_private.h>

unsigned long chess_perft_recursive(const ChessPosition *position, unsigned int depth) {
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
		count += chess_perft_recursive(&position_after_move, depth - 1);
	}

	return count;
}
unsigned long chess_perft(unsigned int depth) {
	ChessPosition position = chess_position_new();
	return chess_perft_recursive(&position, depth);
}
