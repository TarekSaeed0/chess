#include <assert.h>
#include <chess.h>
#include <stdio.h>
#include <stdlib.h>

void chess_square_print(enum chess_square square) {
	char algebraic[3];
	chess_square_to_algebraic(square, algebraic, sizeof(algebraic));
	printf("%s", algebraic);
}
enum chess_square chess_square_scan(void) {
	char string[2];
	(void)scanf(" %c%c", &string[0], &string[1]);

	assert('a' <= string[0] && string[0] <= 'h' && '1' <= string[1] && string[1] <= '8');
	enum chess_file file = (enum chess_file)(CHESS_FILE_A + (string[0] - 'a'));
	enum chess_rank rank = (enum chess_rank)(CHESS_RANK_1 + (string[1] - '1'));

	return chess_square_new(file, rank);
}

void chess_position_print(const struct chess_position *chess) {
	assert(chess != NULL);

	printf("\033[38;5;254;48;5;233m   ");
	for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
		printf(" %c ", 'a' + (file - CHESS_FILE_A));
	}
	printf("   \033[0m\n");
	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		printf("\033[38;5;254;48;5;233m %c ", '1' + (rank - CHESS_RANK_1));
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square square      = chess_square_new(file, rank);
			enum chess_color square_color = chess_square_color(square);
			if (square_color == CHESS_COLOR_WHITE) {
				printf("\033[38;5;235;48;5;254m");
			} else {
				printf("\033[38;5;254;48;5;235m");
			}
			enum chess_piece piece = chess->board[square];
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
	for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
		printf(" %c ", 'a' + (file - CHESS_FILE_A));
	}
	printf("   \033[0m\n");
}

int main(void) {
	struct chess_position position = chess_position_new();
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
		struct chess_moves moves = chess_moves_generate(&position);
		for (size_t i = 0; i < moves.count; i++) {
			char algebraic[8];
			chess_move_to_algebraic(&position, moves.moves[i], algebraic, sizeof(algebraic));
			printf("%s ", algebraic);
		}
		printf("\n");

		struct chess_move move = { .from = chess_square_scan(), .to = chess_square_scan() };
		if (!chess_move_do(&position, move)) {
			printf("Illegal move.\n");
			continue;
		}
		chess_position_print(&position);
	}
}
