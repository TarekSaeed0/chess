#include <chess.h>

#include <stdio.h>

void chess_print(const struct chess *chess) {
	assert(chess != NULL);

	printf("\033[38;5;254;48;5;233m   ");
	for (uint8_t file = 0; file < 8; file++) {
		printf(" %c ", 'a' + file);
	}
	printf("   \033[0m\n");
	for (enum chess_square square = chess_square_a8; square <= chess_square_h1; square++) {
		if (square & 0x88U) {
			printf("\033[0m\n");
			square += 7;
			continue;
		}

		if (chess_square_get_file(square) == 0) {
			printf("\033[38;5;254;48;5;233m %d ", 8 - chess_square_get_rank(square));
		}

		enum chess_piece_color square_color =
			(chess_square_get_file(square) + chess_square_get_rank(square)) % 2;
		if (square_color == chess_piece_color_white) {
			printf("\033[38;5;235;48;5;254m");
		} else {
			printf("\033[38;5;254;48;5;235m");
		}
		enum chess_piece piece = chess->board[square];
		if (piece == chess_piece_none) {
			printf("   ");
		} else {
			static const char *chess_symbols[][2] = {
				[chess_piece_type_king] = { [false] = "♚", [true] = "♔", },
				[chess_piece_type_queen] = { [false] = "♛", [true] = "♕", },
				[chess_piece_type_bishop] = { [false] = "♝", [true] = "♗", },
				[chess_piece_type_knight] = { [false] = "♞", [true] = "♘", },
				[chess_piece_type_rook] = { [false] = "♜", [true] = "♖", },
				[chess_piece_type_pawn] = { [false] = "♟", [true] = "♙", },
			};

			printf(
				" %s ",
				chess_symbols[chess_piece_get_type(piece)]
							 [chess_piece_get_color(piece) == square_color]
			);
		}

		if (chess_square_get_file(square) == 7) {
			printf("\033[38;5;254;48;5;233m %d ", 8 - chess_square_get_rank(square));
		}
	}
	printf("\033[0m\n\033[38;5;254;48;5;233m   ");
	for (uint8_t file = 0; file < 8; file++) {
		printf(" %c ", 'a' + file);
	}
	printf("   \033[0m\n");
}

void chess_square_print(enum chess_square square) {
	printf("%c%d", 'a' + chess_square_get_file(square), 8 - chess_square_get_rank(square));
}
enum chess_square chess_square_scan(void) {
	char string[2];
	(void)scanf(" %c%c", &string[0], &string[1]);

	assert('a' <= string[0] && string[0] <= 'h' && '1' <= string[1] && string[1] <= '8');

	return chess_square_new((uint8_t)(string[0] - 'a'), (uint8_t)(8 - (string[1] - '0')));
}

int main(void) {
	struct chess chess = chess_new();

	chess_move(
		&chess,
		(struct chess_move){
			.from = chess_square_f2,
			.to = chess_square_f3,
		}
	);
	chess_move(
		&chess,
		(struct chess_move){
			.from = chess_square_e7,
			.to = chess_square_e6,
		}
	);
	chess_move(
		&chess,
		(struct chess_move){
			.from = chess_square_b1,
			.to = chess_square_c3,
		}
	);
	chess_move(
		&chess,
		(struct chess_move){
			.from = chess_square_d8,
			.to = chess_square_h4,
		}
	);

	chess_print(&chess);

	while (true) {
		enum chess_status status = chess_get_status(&chess);
		switch (status) {
			case chess_status_ongoing: printf("Ongoing\n"); break;
			case chess_status_check: printf("Check\n"); break;
			case chess_status_checkmate: printf("Checkmate\n"); break;
		}

		struct chess_moves moves = chess_get_legal_moves(
			&chess,
			(struct chess_get_moves_filter){
				.flags.color = true,
				.color = chess.turn,
			}
		);
		for (size_t i = 0; i < moves.count; i++) {
			chess_square_print(moves.moves[i].from);
			chess_square_print(moves.moves[i].to);
			printf(" ");
		}
		printf("\n");

		struct chess_move move = { .from = chess_square_scan(), .to = chess_square_scan() };
		if (!chess_move(&chess, move)) {
			continue;
		}
		chess_print(&chess);
	}

	(void)chess;
}
