#include <chess.h>
#include <stdio.h>

int main(void) {
	struct chess_position position =
		chess_postion_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	char *fen = chess_postion_fen(position);
	printf("%s\n", fen);
}
