#include <chess/square.h>

#include <chess/file.h>
#include <chess/macros_private.h>
#include <chess/offset.h>
#include <chess/piece.h>
#include <chess/position.h>
#include <chess/rank.h>

#include <assert.h>
#include <stdio.h>

void chess_square_debug(ChessSquare square) {
	if (square == CHESS_SQUARE_NONE) {
		printf("CHESS_SQUARE_NONE");
	} else if (chess_square_is_valid(square)) {
		ChessFile file = chess_square_file(square);
		ChessRank rank = chess_square_rank(square);

		printf("CHESS_SQUARE_%c%c", 'A' + (file - CHESS_FILE_A), '1' + (rank - CHESS_RANK_1));
	} else {
		printf("(ChessSquare)%d", square);
	}
}
bool chess_square_is_valid(ChessSquare square) {
	return (square & 0x88U) == 0x88U;
}
ChessColor chess_square_color(ChessSquare square) {
	return (chess_square_file(square) + chess_square_rank(square)) % 2U ? CHESS_COLOR_BLACK : CHESS_COLOR_WHITE;
}
size_t chess_square_from_algebraic(ChessSquare *square, const char *string) {
	assert(square != CHESS_NULL && string != CHESS_NULL);

	size_t total_read = 0;

	ChessFile file    = CHESS_FILE_NONE;
	READ(chess_file_from_algebraic, &file);

	ChessRank rank = CHESS_RANK_NONE;
	READ(chess_rank_from_algebraic, &rank);

	*square = chess_square_new(file, rank);

	assert(chess_square_is_valid(*square));

	return total_read;
}
size_t chess_square_to_algebraic(ChessSquare square, char *string, size_t string_size) {
	assert(chess_square_is_valid(square));

	size_t total_written = 0;

	WRITE(chess_file_to_algebraic, chess_square_file(square));
	WRITE(chess_rank_to_algebraic, chess_square_rank(square));

	return total_written;
}
bool chess_square_is_attacked(const ChessPosition *position, ChessSquare square, ChessColor color) {
	assert(chess_position_is_valid(position) && chess_square_is_valid(square));

	for (ChessRank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			ChessSquare attacker_square = chess_square_new(file, rank);

			ChessPiece attacker         = position->board[attacker_square];
			ChessColor attacker_color   = chess_piece_color(attacker);
			if (attacker_color != color) {
				continue;
			}

			// clang-format off
			static CHESS_CONSTEXPR uint8_t attacks[256] = {
			 40,  0,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0,  0, 40,  0,
				0, 40,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0, 40,  0,  0,
				0,  0, 40,  0,  0,  0,  0, 48,  0,  0,  0,  0, 40,  0,  0,  0,
				0,  0,  0, 40,  0,  0,  0, 48,  0,  0,  0, 40,  0,  0,  0,  0,
				0,  0,  0,  0, 40,  0,  0, 48,  0,  0, 40,  0,  0,  0,  0,  0,
				0,  0,  0,  0,  0, 40,  4, 48,  4, 40,  0,  0,  0,  0,  0,  0,
				0,  0,  0,  0,  0,  4,106,112,106,  4,  0,  0,  0,  0,  0,  0,
			 48, 48, 48, 48, 48, 48,112,  0,112, 48, 48, 48, 48, 48, 48,  0,
				0,  0,  0,  0,  0,  4,106,112,106,  4,  0,  0,  0,  0,  0,  0,
				0,  0,  0,  0,  0, 40,  4, 48,  4, 40,  0,  0,  0,  0,  0,  0,
				0,  0,  0,  0, 40,  0,  0, 48,  0,  0, 40,  0,  0,  0,  0,  0,
				0,  0,  0, 40,  0,  0,  0, 48,  0,  0,  0, 40,  0,  0,  0,  0,
				0,  0, 40,  0,  0,  0,  0, 48,  0,  0,  0,  0, 40,  0,  0,  0,
				0, 40,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0, 40,  0,  0,
			 40,  0,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0,  0, 40,  0,
				0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			};
			// clang-format on

			int8_t difference            = (int8_t)(square - attacker_square);
			uint8_t index                = (uint8_t)(difference + 0x77);

			ChessPieceType attacker_type = chess_piece_type(attacker);
			if (attacks[index] & (1U << attacker_type)) {
				switch (attacker_type) {
					case CHESS_PIECE_TYPE_PAWN: {
						if (attacker_color == CHESS_COLOR_WHITE ? difference > 0 : difference < 0) {
							return true;
						}
					} break;
					case CHESS_PIECE_TYPE_KNIGHT:
					case CHESS_PIECE_TYPE_KING: {
						return true;
					} break;
					case CHESS_PIECE_TYPE_ROOK:
					case CHESS_PIECE_TYPE_BISHOP:
					case CHESS_PIECE_TYPE_QUEEN: {
						// clang-format off
						static CHESS_CONSTEXPR chess_offset directions[256] = {
							-17,  0,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,  0,-15,  0,
								0,-17,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,-15,  0,  0,
								0,  0,-17,  0,  0,  0,  0,-16,  0,  0,  0,  0,-15,  0,  0,  0,
								0,  0,  0,-17,  0,  0,  0,-16,  0,  0,  0,-15,  0,  0,  0,  0,
								0,  0,  0,  0,-17,  0,  0,-16,  0,  0,-15,  0,  0,  0,  0,  0,
								0,  0,  0,  0,  0,-17,  0,-16,  0,-15,  0,  0,  0,  0,  0,  0,
								0,  0,  0,  0,  0,  0,-17,-16,-15,  0,  0,  0,  0,  0,  0,  0,
							 -1, -1, -1, -1, -1, -1, -1,  0,  1,  1,  1,  1,  1,  1,  1,  0,
								0,  0,  0,  0,  0,  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,
								0,  0,  0,  0,  0, 15,  0, 16,  0, 17,  0,  0,  0,  0,  0,  0,
								0,  0,  0,  0, 15,  0,  0, 16,  0,  0, 17,  0,  0,  0,  0,  0,
								0,  0,  0, 15,  0,  0,  0, 16,  0,  0,  0, 17,  0,  0,  0,  0,
								0,  0, 15,  0,  0,  0,  0, 16,  0,  0,  0,  0, 17,  0,  0,  0,
								0, 15,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0, 17,  0,  0,
							 15,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 17,  0,
								0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
						};
						// clang-format on

						chess_offset direction = directions[index];
						ChessSquare current    = (ChessSquare)(attacker_square + direction);
						while (true) {
							if (current == square) {
								return true;
							}
							if (position->board[current] != CHESS_PIECE_NONE) {
								break;
							}
							current += direction;
						}
					} break;
					default: assert(false);
				}
			}
		}
	}

	return false;
}
