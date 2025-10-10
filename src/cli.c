#include "chess/move_private.h"
#include <assert.h>
#include <chess.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double chess_position_negamax(const ChessPosition *position, int depth, double alpha, double beta, int color) {
	if (depth == 0 || chess_position_is_checkmate(position) || chess_position_is_stalemate(position)) {
		return color * chess_position_evaluate(position);
	}

	double maximum_value = -INFINITY;
	ChessMoves moves     = chess_moves_generate(position);
	for (size_t i = 0; i < moves.count; i++) {
		ChessPosition position_after_move = *position;
		chess_move_do_unchecked(&position_after_move, moves.moves[i]);
		double value = -chess_position_negamax(&position_after_move, depth - 1, -beta, -alpha, -color);
		if (value > maximum_value) {
			maximum_value = value;
			if (value > alpha) {
				alpha = value;
			}
		}
		if (alpha >= beta) {
			break;
		}
	}
	return maximum_value;
}
ChessMove chess_position_best_move(const ChessPosition *position, int depth) {
	assert(position != NULL);
	assert(depth > 0);

	ChessMove best_move = {
		.from           = CHESS_SQUARE_NONE,
		.to             = CHESS_SQUARE_NONE,
		.promotion_type = CHESS_PIECE_TYPE_NONE
	};
	double best_value = -INFINITY;
	ChessMoves moves  = chess_moves_generate(position);
	for (size_t i = 0; i < moves.count; i++) {
		ChessPosition position_after_move = *position;
		chess_move_do_unchecked(&position_after_move, moves.moves[i]);
		double value = -chess_position_negamax(
		    &position_after_move,
		    depth - 1,
		    -INFINITY,
		    INFINITY,
		    (chess_position_side_to_move(position) == CHESS_COLOR_WHITE ? 1 : -1)
		);
		if (value > best_value) {
			best_value = value;
			best_move  = moves.moves[i];
		}
	}

	return best_move;
}

void chess_position_print(const ChessPosition *chess) {
	assert(chess != NULL);

	printf("\033[38;5;254;48;5;233m   ");
	for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
		char string[2];
		chess_file_to_algebraic(file, string, sizeof(string));
		printf(" %s ", string);
	}
	printf("   \033[0m\n");
	for (ChessRank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		char string[2];
		chess_rank_to_algebraic(rank, string, sizeof(string));
		printf("\033[38;5;254;48;5;233m %s ", string);
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
		chess_rank_to_algebraic(rank, string, sizeof(string));
		printf("\033[38;5;254;48;5;233m %s ", string);
		if (rank != CHESS_RANK_1) {
			printf("\033[0m\n");
		}
	}
	printf("\033[0m\n\033[38;5;254;48;5;233m   ");
	for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
		char string[2];
		chess_file_to_algebraic(file, string, sizeof(string));
		printf(" %s ", string);
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
		printf("Score: %lg\n", chess_position_evaluate(&position));
		printf("%s to move.\n", chess_position_side_to_move(&position) == CHESS_COLOR_WHITE ? "White" : "Black");
		printf("Available moves: ");
		ChessMoves moves = chess_moves_generate(&position);
		for (size_t i = 0; i < moves.count; i++) {
			char string[8];
			chess_move_to_algebraic(&position, moves.moves[i], string, sizeof(string));
			printf("%s ", string);
		}
		printf("\n");

		if (chess_position_side_to_move(&position) == CHESS_COLOR_BLACK) {
			printf("Black is thinking...\n");
			clock_t start       = clock();
			ChessMove best_move = chess_position_best_move(&position, 4);
			clock_t end         = clock();
			double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
			char string[8];
			chess_move_to_algebraic(&position, best_move, string, sizeof(string));
			printf("Black plays: %s (computed in %.2f milliseconds)\n", string, elapsed_time);
			chess_move_do(&position, best_move);
		} else {
			char string[64];
			if (fgets(string, sizeof(string), stdin) == NULL) {
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
		}

		chess_position_print(&position);
	}

	chess_position_drop(&position);
}
