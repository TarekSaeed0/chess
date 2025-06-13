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
	return (square & 0x88U) == 0;
}
ChessSquare chess_square_new(ChessFile file, ChessRank rank) {
	assert(chess_file_is_valid(file) && chess_rank_is_valid(rank));

	ChessSquare square = file | rank << 4U;

	assert(chess_square_is_valid(square));

	return square;
}
ChessFile chess_square_file(ChessSquare square) {
	assert(chess_square_is_valid(square));

	return (ChessFile)(square & 0xFU);
}
ChessRank chess_square_rank(ChessSquare square) {
	assert(chess_square_is_valid(square));

	return (ChessRank)(square >> 4U);
}
ChessColor chess_square_color(ChessSquare square) {
	return (chess_square_file(square) + chess_square_rank(square)) % 2U ? CHESS_COLOR_BLACK : CHESS_COLOR_WHITE;
}
size_t chess_square_from_algebraic(ChessSquare *square, const char *string) {
	assert(square != CHESS_NULL && string != CHESS_NULL);

	size_t total_read = 0;

	ChessFile file    = CHESS_FILE_NONE;
	CHESS_READ(chess_file_from_algebraic, &file);

	ChessRank rank = CHESS_RANK_NONE;
	CHESS_READ(chess_rank_from_algebraic, &rank);

	*square = chess_square_new(file, rank);

	assert(chess_square_is_valid(*square));

	return total_read;
}
size_t chess_square_to_algebraic(ChessSquare square, char *string, size_t string_size) {
	assert(chess_square_is_valid(square));

	size_t total_written = 0;

	CHESS_WRITE(chess_file_to_algebraic, chess_square_file(square));
	CHESS_WRITE(chess_rank_to_algebraic, chess_square_rank(square));

	return total_written;
}
bool chess_square_is_attacked(const ChessPosition *position, ChessSquare square, ChessColor color) {
	assert(chess_position_is_valid(position) && chess_square_is_valid(square));

	for (ChessSquare attacker_square = CHESS_SQUARE_A1; attacker_square <= CHESS_SQUARE_H8; attacker_square++) {
		if (!chess_square_is_valid(attacker_square)) {
			attacker_square += CHESS_SQUARE_A2 - (CHESS_SQUARE_H1 + 1) - 1;
			continue;
		}

		ChessPiece attacker       = position->board[attacker_square];
		ChessColor attacker_color = chess_piece_color(attacker);
		if (attacker_color != color) {
			continue;
		}

		static CHESS_CONSTEXPR uint8_t attacks[256] = {
			[CHESS_OFFSET_NORTH + 0x77]                         = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
			[2 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[3 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[4 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[5 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[6 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[7 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,

			[CHESS_OFFSET_EAST + 0x77]                          = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
			[2 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[3 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[4 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[5 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[6 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[7 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,

			[CHESS_OFFSET_SOUTH + 0x77]                         = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
			[2 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[3 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[4 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[5 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[6 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[7 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,

			[CHESS_OFFSET_WEST + 0x77]                          = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
			[2 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[3 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[4 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[5 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[6 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
			[7 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,

			[CHESS_OFFSET_NORTH_EAST + 0x77]                    = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
			[2 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[3 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[4 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[5 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[6 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[7 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,

			[CHESS_OFFSET_SOUTH_EAST + 0x77]                    = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
			[2 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[3 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[4 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[5 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[6 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[7 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,

			[CHESS_OFFSET_SOUTH_WEST + 0x77]                    = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
			[2 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[3 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[4 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[5 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[6 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[7 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,

			[CHESS_OFFSET_NORTH_WEST + 0x77]                    = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
			[2 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[3 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[4 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[5 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[6 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
			[7 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,

			[CHESS_OFFSET_WEST + 2 * CHESS_OFFSET_SOUTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
			[CHESS_OFFSET_EAST + 2 * CHESS_OFFSET_SOUTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
			[2 * CHESS_OFFSET_WEST + CHESS_OFFSET_SOUTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
			[2 * CHESS_OFFSET_EAST + CHESS_OFFSET_SOUTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
			[2 * CHESS_OFFSET_WEST + CHESS_OFFSET_NORTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
			[2 * CHESS_OFFSET_EAST + CHESS_OFFSET_NORTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
			[CHESS_OFFSET_WEST + 2 * CHESS_OFFSET_NORTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
			[CHESS_OFFSET_EAST + 2 * CHESS_OFFSET_NORTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
		};

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
					static CHESS_CONSTEXPR ChessOffset directions[256] = {
						[CHESS_OFFSET_NORTH + 0x77]          = CHESS_OFFSET_NORTH,
						[2 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
						[3 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
						[4 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
						[5 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
						[6 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
						[7 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,

						[CHESS_OFFSET_EAST + 0x77]           = CHESS_OFFSET_EAST,
						[2 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
						[3 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
						[4 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
						[5 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
						[6 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
						[7 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,

						[CHESS_OFFSET_SOUTH + 0x77]          = CHESS_OFFSET_SOUTH,
						[2 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
						[3 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
						[4 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
						[5 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
						[6 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
						[7 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,

						[CHESS_OFFSET_WEST + 0x77]           = CHESS_OFFSET_WEST,
						[2 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
						[3 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
						[4 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
						[5 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
						[6 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
						[7 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,

						[CHESS_OFFSET_NORTH_EAST + 0x77]     = CHESS_OFFSET_NORTH_EAST,
						[2 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
						[3 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
						[4 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
						[5 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
						[6 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
						[7 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,

						[CHESS_OFFSET_SOUTH_EAST + 0x77]     = CHESS_OFFSET_SOUTH_EAST,
						[2 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
						[3 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
						[4 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
						[5 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
						[6 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
						[7 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,

						[CHESS_OFFSET_SOUTH_WEST + 0x77]     = CHESS_OFFSET_SOUTH_WEST,
						[2 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
						[3 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
						[4 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
						[5 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
						[6 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
						[7 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,

						[CHESS_OFFSET_NORTH_WEST + 0x77]     = CHESS_OFFSET_NORTH_WEST,
						[2 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
						[3 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
						[4 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
						[5 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
						[6 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
						[7 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
					};

					ChessOffset direction = directions[index];
					ChessSquare current   = (ChessSquare)(attacker_square + direction);
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

	return false;
}
