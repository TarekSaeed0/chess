#include <assert.h>
#include <chess.h>
#include <stdio.h>
#include <stdlib.h>

void chess_position_print(const ChessPosition *chess) {
	assert(chess != NULL);

	printf("\033[38;5;254;48;5;233m   ");
	for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
		printf(" %c ", 'a' + (file - CHESS_FILE_A));
	}
	printf("   \033[0m\n");
	for (ChessRank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		printf("\033[38;5;254;48;5;233m %c ", '1' + (rank - CHESS_RANK_1));
		for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			ChessSquare square      = chess_square_new(file, rank);
			ChessColor square_color = chess_square_color(square);
			if (square_color == CHESS_COLOR_WHITE) {
				printf("\033[38;5;235;48;5;254m");
			} else {
				printf("\033[38;5;254;48;5;235m");
			}
			ChessPiece piece = chess->board[square];
			if (piece == CHESS_PIECE_NONE) {
				printf("   ");
			} else {
				static const char *chess_symbols[][2] = {
					[CHESS_PIECE_TYPE_PAWN] = {
					    [false] = "♟",
					    [true]  = "♙",
					},
					[CHESS_PIECE_TYPE_KNIGHT] = {
					    [false] = "♞",
					    [true]  = "♘",
					},
					[CHESS_PIECE_TYPE_BISHOP] = {
					    [false] = "♝",
					    [true]  = "♗",
					},
					[CHESS_PIECE_TYPE_ROOK] = {
					    [false] = "♜",
					    [true]  = "♖",
					},
					[CHESS_PIECE_TYPE_QUEEN] = {
					    [false] = "♛",
					    [true]  = "♕",
					},
					[CHESS_PIECE_TYPE_KING] = {
					    [false] = "♚",
					    [true]  = "♔",
					},
				};

				printf(" %s ", chess_symbols[chess_piece_type(piece)][chess_piece_color(piece) == square_color]);
			}
		}
		printf("\033[38;5;254;48;5;233m %c ", '1' + (rank - CHESS_RANK_1));
		if (rank != CHESS_RANK_1) {
			printf("\033[0m\n");
		}
	}
	printf("\033[0m\n\033[38;5;254;48;5;233m   ");
	for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
		printf(" %c ", 'a' + (file - CHESS_FILE_A));
	}
	printf("   \033[0m\n");
}

int main(void) {
	ChessPosition position = chess_position_new();
	if (!chess_position_from_fen(&position, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")) {
		(void)fprintf(stderr, "Error: Invalid FEN position\n");
		return EXIT_FAILURE;
	}

	chess_position_print(&position);
	while (true) {
		char fen[128];
		chess_position_to_fen(&position, fen, sizeof(fen));
		printf("Position: %s\n", fen);
		if (chess_position_is_checkmate(&position)) {
			printf("Checkmate!\n");
			break;
		}
		if (chess_position_is_check(&position)) {
			printf("Check!\n");
		}
		printf("%s to move.\n", position.side_to_move == CHESS_COLOR_WHITE ? "White" : "Black");
		printf("Available moves: ");
		ChessMoves moves = chess_moves_generate(&position);
		for (size_t i = 0; i < moves.count; i++) {
			char string[8];
			chess_move_to_algebraic(&position, moves.moves[i], string, sizeof(string));
			printf("%s ", string);
		}
		printf("\n");

		char string[64];
		if (scanf("%63s", string) != 1) {
			(void)fprintf(stderr, "Failed to read move.\n");
			continue;
		}
		ChessMove move;
		if (chess_move_from_algebraic(&position, &move, string) == 0) {
			(void)fprintf(stderr, "Invalid move: %s\n", string);
			continue;
		}
		if (!chess_move_do(&position, move)) {
			(void)fprintf(stderr, "Illegal move.\n");
			continue;
		}
		chess_position_print(&position);
	}

	chess_position_drop(&position);
}
