#include <chess.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

bool chess_color_is_valid(enum chess_color color) {
	switch (color) {
		case CHESS_COLOR_WHITE:
		case CHESS_COLOR_BLACK: return true;
		default: return false;
	}
}
enum chess_color chess_color_opposite(enum chess_color color) {
	switch (color) {
		case CHESS_COLOR_NONE: return CHESS_COLOR_NONE;
		case CHESS_COLOR_WHITE: return CHESS_COLOR_BLACK;
		case CHESS_COLOR_BLACK: return CHESS_COLOR_WHITE;
		default: assert(false);
	}
}

bool chess_piece_is_valid(enum chess_piece piece) {
	switch (piece) {
		case CHESS_PIECE_WHITE_PAWN:
		case CHESS_PIECE_WHITE_KNIGHT:
		case CHESS_PIECE_WHITE_BISHOP:
		case CHESS_PIECE_WHITE_ROOK:
		case CHESS_PIECE_WHITE_QUEEN:
		case CHESS_PIECE_WHITE_KING:
		case CHESS_PIECE_BLACK_PAWN:
		case CHESS_PIECE_BLACK_KNIGHT:
		case CHESS_PIECE_BLACK_BISHOP:
		case CHESS_PIECE_BLACK_ROOK:
		case CHESS_PIECE_BLACK_QUEEN:
		case CHESS_PIECE_BLACK_KING: return true;
		default: return false;
	}
}
size_t chess_piece_from_algebraic(enum chess_piece *piece, const char *algebraic) {
	assert(piece != nullptr && algebraic != nullptr);

	switch (algebraic[0]) {
		case 'P': *piece = CHESS_PIECE_WHITE_PAWN; break;
		case 'N': *piece = CHESS_PIECE_WHITE_KNIGHT; break;
		case 'B': *piece = CHESS_PIECE_WHITE_BISHOP; break;
		case 'R': *piece = CHESS_PIECE_WHITE_ROOK; break;
		case 'Q': *piece = CHESS_PIECE_WHITE_QUEEN; break;
		case 'K': *piece = CHESS_PIECE_WHITE_KING; break;
		case 'p': *piece = CHESS_PIECE_BLACK_PAWN; break;
		case 'n': *piece = CHESS_PIECE_BLACK_KNIGHT; break;
		case 'b': *piece = CHESS_PIECE_BLACK_BISHOP; break;
		case 'r': *piece = CHESS_PIECE_BLACK_ROOK; break;
		case 'q': *piece = CHESS_PIECE_BLACK_QUEEN; break;
		case 'k': *piece = CHESS_PIECE_BLACK_KING; break;
		default: return 0;
	}

	return 1;
}
size_t chess_piece_to_algebraic(enum chess_piece piece, char *algebraic, size_t algebraic_size) {
	size_t total_written = 0;
#define APPEND_TO_ALGEBRAIC(...)                                                                                       \
	do {                                                                                                                 \
		int written = snprintf(algebraic, algebraic_size, __VA_ARGS__);                                                    \
		assert(written >= 0);                                                                                              \
		if ((size_t)written < algebraic_size) {                                                                            \
			if (algebraic != NULL) {                                                                                         \
				algebraic += written;                                                                                          \
			}                                                                                                                \
			algebraic_size -= (size_t)written;                                                                               \
		} else {                                                                                                           \
			algebraic_size = 0;                                                                                              \
		}                                                                                                                  \
		total_written += (size_t)written;                                                                                  \
	} while (0)

	switch (piece) {
		case CHESS_PIECE_WHITE_PAWN: APPEND_TO_ALGEBRAIC("P"); break;
		case CHESS_PIECE_WHITE_KNIGHT: APPEND_TO_ALGEBRAIC("N"); break;
		case CHESS_PIECE_WHITE_BISHOP: APPEND_TO_ALGEBRAIC("B"); break;
		case CHESS_PIECE_WHITE_ROOK: APPEND_TO_ALGEBRAIC("R"); break;
		case CHESS_PIECE_WHITE_QUEEN: APPEND_TO_ALGEBRAIC("Q"); break;
		case CHESS_PIECE_WHITE_KING: APPEND_TO_ALGEBRAIC("K"); break;
		case CHESS_PIECE_BLACK_PAWN: APPEND_TO_ALGEBRAIC("p"); break;
		case CHESS_PIECE_BLACK_KNIGHT: APPEND_TO_ALGEBRAIC("n"); break;
		case CHESS_PIECE_BLACK_BISHOP: APPEND_TO_ALGEBRAIC("b"); break;
		case CHESS_PIECE_BLACK_ROOK: APPEND_TO_ALGEBRAIC("r"); break;
		case CHESS_PIECE_BLACK_QUEEN: APPEND_TO_ALGEBRAIC("q"); break;
		case CHESS_PIECE_BLACK_KING: APPEND_TO_ALGEBRAIC("k"); break;
		default: assert(false);
	}

	return total_written;
#undef APPEND_TO_ALGEBRAIC
}

enum chess_offset : int8_t {
	CHESS_OFFSET_NORTH      = CHESS_SQUARE_A2 - CHESS_SQUARE_A1,
	CHESS_OFFSET_EAST       = CHESS_SQUARE_B1 - CHESS_SQUARE_A1,
	CHESS_OFFSET_SOUTH      = -CHESS_OFFSET_NORTH,
	CHESS_OFFSET_WEST       = -CHESS_OFFSET_EAST,

	CHESS_OFFSET_NORTH_EAST = CHESS_OFFSET_NORTH + CHESS_OFFSET_EAST,
	CHESS_OFFSET_SOUTH_EAST = CHESS_OFFSET_SOUTH + CHESS_OFFSET_EAST,
	CHESS_OFFSET_SOUTH_WEST = CHESS_OFFSET_SOUTH + CHESS_OFFSET_WEST,
	CHESS_OFFSET_NORTH_WEST = CHESS_OFFSET_NORTH + CHESS_OFFSET_WEST,
};

enum chess_color chess_square_color(enum chess_square square) {
	return (chess_square_file(square) + chess_square_rank(square)) % 2U ? CHESS_COLOR_BLACK : CHESS_COLOR_WHITE;
}
bool chess_square_is_valid(enum chess_square square) {
	return (square & 0x88U) == 0x88U;
}
size_t chess_square_from_algebraic(enum chess_square *square, const char *algebraic) {
	assert(square != nullptr && algebraic != nullptr);

	if (algebraic[0] < 'a' || 'h' < algebraic[0] || algebraic[1] < '1' || '8' < algebraic[1]) {
		return 0;
	}

	enum chess_file file = (enum chess_file)(CHESS_FILE_A + (algebraic[0] - 'a'));
	enum chess_rank rank = (enum chess_rank)(CHESS_RANK_1 + (algebraic[1] - '1'));
	*square              = chess_square_new(file, rank);

	assert(chess_square_is_valid(*square));

	return 2;
}
size_t chess_square_to_algebraic(enum chess_square square, char *algebraic, size_t algebraic_size) {
	assert(chess_square_is_valid(square));

	size_t total_written = 0;
#define APPEND_TO_ALGEBRAIC(...)                                                                                       \
	do {                                                                                                                 \
		int written = snprintf(algebraic, algebraic_size, __VA_ARGS__);                                                    \
		assert(written >= 0);                                                                                              \
		if ((size_t)written < algebraic_size) {                                                                            \
			if (algebraic != NULL) {                                                                                         \
				algebraic += written;                                                                                          \
			}                                                                                                                \
			algebraic_size -= (size_t)written;                                                                               \
		} else {                                                                                                           \
			algebraic_size = 0;                                                                                              \
		}                                                                                                                  \
		total_written += (size_t)written;                                                                                  \
	} while (0)

	enum chess_file file = chess_square_file(square);
	enum chess_rank rank = chess_square_rank(square);
	APPEND_TO_ALGEBRAIC("%c", (char)('a' + (file - CHESS_FILE_A)));
	APPEND_TO_ALGEBRAIC("%c", (char)('1' + (rank - CHESS_RANK_1)));

	return total_written;
#undef APPEND_TO_ALGEBRAIC
}
bool chess_square_is_attacked(const struct chess_position *position, enum chess_square square, enum chess_color color) {
	assert(chess_position_is_valid(position) && chess_square_is_valid(square));

	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square attacker_square = chess_square_new(file, rank);

			enum chess_piece attacker         = position->board[attacker_square];
			enum chess_color attacker_color   = chess_piece_color(attacker);
			if (attacker_color != color) {
				continue;
			}

			// clang-format off
			static const uint8_t attacks[256] = {
				 40,  0,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0,  0, 40,  0,
					0, 40,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0, 40,  0,  0,
					0,  0, 40,  0,  0,  0,  0, 48,  0,  0,  0,  0, 40,  0,  0,  0,
					0,  0,  0, 40,  0,  0,  0, 48,  0,  0,  0, 40,  0,  0,  0,  0,
					0,  0,  0,  0, 40,  0,  0, 48,  0,  0, 40,  0,  0,  0,  0,  0,
					0,  0,  0,  0,  0, 40,  4, 48,  4, 40,  0,  0,  0,  0,  0,  0,
					0,  0,  0,  0,  0,  4,106,114,106,  4,  0,  0,  0,  0,  0,  0,
				 48, 48, 48, 48, 48, 48,112,  0,112, 48, 48, 48, 48, 48, 48,  0,
					0,  0,  0,  0,  0,  4,106,114,106,  4,  0,  0,  0,  0,  0,  0,
					0,  0,  0,  0,  0, 40,  4, 48,  4, 40,  0,  0,  0,  0,  0,  0,
					0,  0,  0,  0, 40,  0,  0, 48,  0,  0, 40,  0,  0,  0,  0,  0,
					0,  0,  0, 40,  0,  0,  0, 48,  0,  0,  0, 40,  0,  0,  0,  0,
					0,  0, 40,  0,  0,  0,  0, 48,  0,  0,  0,  0, 40,  0,  0,  0,
					0, 40,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0, 40,  0,  0,
				 40,  0,  0,  0,  0,  0,  0, 48,  0,  0,  0,  0,  0,  0, 40,  0,
					0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			};
			// clang-format on

			int8_t difference                   = (int8_t)(square - attacker_square);
			uint8_t index                       = (uint8_t)(difference + 0x77);

			enum chess_piece_type attacker_type = chess_piece_type(attacker);
			if (attacks[index] & (1U << attacker_type)) {
				switch (attacker_type) {

					case CHESS_PIECE_TYPE_PAWN: {
						if ((difference > 0 && attacker_color == CHESS_COLOR_WHITE) ||
						    (difference < 0 && attacker_color == CHESS_COLOR_BLACK)) {
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
						static const enum chess_offset directions[256] = {
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

						enum chess_offset direction = directions[index];
						enum chess_square current   = (enum chess_square)(attacker_square + direction);
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

struct chess_position chess_position_new(void) {
	return (struct chess_position) {
		.board = { 
			[CHESS_SQUARE_A8] = CHESS_PIECE_BLACK_ROOK,
			[CHESS_SQUARE_B8] = CHESS_PIECE_BLACK_KNIGHT,
			[CHESS_SQUARE_C8] = CHESS_PIECE_BLACK_BISHOP,
			[CHESS_SQUARE_D8] = CHESS_PIECE_BLACK_QUEEN,
			[CHESS_SQUARE_E8] = CHESS_PIECE_BLACK_KING,
			[CHESS_SQUARE_F8] = CHESS_PIECE_BLACK_BISHOP,
			[CHESS_SQUARE_G8] = CHESS_PIECE_BLACK_KNIGHT,
			[CHESS_SQUARE_H8] = CHESS_PIECE_BLACK_ROOK,

			[CHESS_SQUARE_A7] = CHESS_PIECE_BLACK_PAWN,
			[CHESS_SQUARE_B7] = CHESS_PIECE_BLACK_PAWN,
			[CHESS_SQUARE_C7] = CHESS_PIECE_BLACK_PAWN,
			[CHESS_SQUARE_D7] = CHESS_PIECE_BLACK_PAWN,
			[CHESS_SQUARE_E7] = CHESS_PIECE_BLACK_PAWN,
			[CHESS_SQUARE_F7] = CHESS_PIECE_BLACK_PAWN,
			[CHESS_SQUARE_G7] = CHESS_PIECE_BLACK_PAWN,
			[CHESS_SQUARE_H7] = CHESS_PIECE_BLACK_PAWN,

			[CHESS_SQUARE_A2] = CHESS_PIECE_WHITE_PAWN,
			[CHESS_SQUARE_B2] = CHESS_PIECE_WHITE_PAWN,
			[CHESS_SQUARE_C2] = CHESS_PIECE_WHITE_PAWN,
			[CHESS_SQUARE_D2] = CHESS_PIECE_WHITE_PAWN,
			[CHESS_SQUARE_E2] = CHESS_PIECE_WHITE_PAWN,
			[CHESS_SQUARE_F2] = CHESS_PIECE_WHITE_PAWN,
			[CHESS_SQUARE_G2] = CHESS_PIECE_WHITE_PAWN,
			[CHESS_SQUARE_H2] = CHESS_PIECE_WHITE_PAWN,

			[CHESS_SQUARE_A1] = CHESS_PIECE_WHITE_ROOK,
			[CHESS_SQUARE_B1] = CHESS_PIECE_WHITE_KNIGHT,
			[CHESS_SQUARE_C1] = CHESS_PIECE_WHITE_BISHOP,
			[CHESS_SQUARE_D1] = CHESS_PIECE_WHITE_QUEEN,
			[CHESS_SQUARE_E1] = CHESS_PIECE_WHITE_KING,
			[CHESS_SQUARE_F1] = CHESS_PIECE_WHITE_BISHOP,
			[CHESS_SQUARE_G1] = CHESS_PIECE_WHITE_KNIGHT,
			[CHESS_SQUARE_H1] = CHESS_PIECE_WHITE_ROOK,
		},
		.side_to_move = CHESS_COLOR_WHITE,
		.castling_rights = CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE | CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE,
		.en_passant_square = CHESS_SQUARE_NONE,
		.half_move_clock = 0,
		.full_move_number = 1,
		.king_squares = {
			[CHESS_COLOR_WHITE] = CHESS_SQUARE_E1,
			[CHESS_COLOR_BLACK] = CHESS_SQUARE_E8,
		}
	};
}
bool chess_position_is_valid(const struct chess_position *position) {
	if (position == nullptr) {
		return false;
	}

	if (!chess_color_is_valid(position->side_to_move)) {
		return false;
	}

	if (!chess_square_is_valid(position->king_squares[CHESS_COLOR_WHITE]) ||
	    position->board[position->king_squares[CHESS_COLOR_WHITE]] != CHESS_PIECE_WHITE_KING ||
	    !chess_square_is_valid(position->king_squares[CHESS_COLOR_BLACK]) ||
	    position->board[position->king_squares[CHESS_COLOR_BLACK]] != CHESS_PIECE_BLACK_KING) {
		return false;
	}

	if (((position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE) &&
	     (position->board[CHESS_SQUARE_E1] != CHESS_PIECE_WHITE_KING ||
	      position->board[CHESS_SQUARE_H1] != CHESS_PIECE_WHITE_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE) &&
	     (position->board[CHESS_SQUARE_E1] != CHESS_PIECE_WHITE_KING ||
	      position->board[CHESS_SQUARE_A1] != CHESS_PIECE_WHITE_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE) &&
	     (position->board[CHESS_SQUARE_E8] != CHESS_PIECE_BLACK_KING ||
	      position->board[CHESS_SQUARE_H8] != CHESS_PIECE_BLACK_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE) &&
	     (position->board[CHESS_SQUARE_E8] != CHESS_PIECE_BLACK_KING ||
	      position->board[CHESS_SQUARE_A8] != CHESS_PIECE_BLACK_ROOK))) {
		return false;
	}

	if (position->en_passant_square != CHESS_SQUARE_NONE &&
	    (!chess_square_is_valid(position->en_passant_square) ||
	             position->board[position->en_passant_square] != CHESS_PIECE_NONE ||
	             position->side_to_move == CHESS_COLOR_WHITE
	         ? position->board[position->en_passant_square + CHESS_OFFSET_SOUTH] != CHESS_PIECE_BLACK_PAWN
	         : position->board[position->en_passant_square + CHESS_OFFSET_NORTH] != CHESS_PIECE_WHITE_PAWN)) {
		return false;
	}

	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square square = chess_square_new(file, rank);
			enum chess_piece piece   = position->board[square];
			if (piece != CHESS_PIECE_NONE && !chess_piece_is_valid(piece)) {
				return false;
			}
		}
	}

	return true;
}
size_t chess_position_from_fen(struct chess_position *position, const char *fen) {
	assert(fen != NULL);

	size_t total_read = 0; // NOLINT(readability-identifier-length)

	while (isspace(fen[total_read])) {
		total_read++;
	}

	memset(position->board, CHESS_PIECE_NONE, sizeof(position->board));
	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square square = chess_square_new(file, rank);
			if ('1' <= fen[total_read] && fen[total_read] <= '8') {
				if (fen[total_read] > CHESS_FILE_H - file + '1') {
					return 0;
				}
				file += (fen[total_read] - '1');
				total_read++;
			} else {
				size_t read = chess_piece_from_algebraic(&position->board[square], &fen[total_read]);
				if (read == 0) {
					return 0;
				}
				total_read += read;

				if (chess_piece_type(position->board[square]) == CHESS_PIECE_TYPE_KING) {
					position->king_squares[chess_piece_color(position->board[square])] = square;
				}
			}
		}
		if (rank != CHESS_RANK_1) {
			if (fen[total_read] != '/') {
				return 0;
			}
			total_read++;
		}
	}

	if (!isspace(fen[total_read])) {
		return 0;
	}
	while (isspace(fen[total_read])) {
		total_read++;
	}

	if (fen[total_read] == 'w') {
		position->side_to_move = CHESS_COLOR_WHITE;
	} else if (fen[total_read] == 'b') {
		position->side_to_move = CHESS_COLOR_BLACK;
	} else {
		return 0;
	}
	total_read++;

	if (!isspace(fen[total_read])) {
		return 0;
	}
	while (isspace(fen[total_read])) {
		total_read++;
	}

	position->castling_rights = CHESS_CASTLING_RIGHTS_NONE;
	while (fen[total_read] != '\0' && fen[total_read] != ' ') {
		switch (fen[total_read]) {
			case 'K': position->castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE; break;
			case 'Q': position->castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE; break;
			case 'k': position->castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE; break;
			case 'q': position->castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE; break;
			case '-': break;
			default: return 0;
		}
		total_read++;
	}

	if (!isspace(fen[total_read])) {
		return 0;
	}
	while (isspace(fen[total_read])) {
		total_read++;
	}

	if (fen[total_read] == '-') {
		position->en_passant_square = CHESS_SQUARE_NONE;
		total_read++;
	} else {
		size_t read = chess_square_from_algebraic(&position->en_passant_square, &fen[total_read]);
		if (read == 0) {
			return 0;
		}
		total_read += read;
	}

	if (!isspace(fen[total_read])) {
		return 0;
	}
	while (isspace(fen[total_read])) {
		total_read++;
	}

	errno                = 0;
	char *end            = nullptr;
	unsigned long number = strtoul(fen + total_read, &end, 10);
	if (end == fen + total_read || errno != 0 || number > UINT_MAX) {
		return 0;
	}
	position->half_move_clock = (unsigned int)number;
	total_read                = (size_t)(end - fen);

	if (!isspace(fen[total_read])) {
		return 0;
	}
	while (isspace(fen[total_read])) {
		total_read++;
	}

	errno  = 0;
	end    = nullptr;
	number = strtoul(fen + total_read, &end, 10);
	if (end == fen + total_read || errno != 0 || number > UINT_MAX) {
		return 0;
	}
	position->full_move_number = (unsigned int)number;
	total_read                 = (size_t)(end - fen);

	return total_read;
}
size_t chess_position_to_fen(const struct chess_position *position, char *fen, size_t fen_size) {
	assert(position != nullptr);

	size_t total_written = 0;
#define APPEND_TO_FEN(...)                                                                                             \
	do {                                                                                                                 \
		int written = snprintf(fen, fen_size, __VA_ARGS__);                                                                \
		assert(written >= 0);                                                                                              \
		if ((size_t)written < fen_size) {                                                                                  \
			if (fen != NULL) {                                                                                               \
				fen += written;                                                                                                \
			}                                                                                                                \
			fen_size -= (size_t)written;                                                                                     \
		} else {                                                                                                           \
			fen_size = 0;                                                                                                    \
		}                                                                                                                  \
		total_written += (size_t)written;                                                                                  \
	} while (0)

	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_piece piece = position->board[chess_square_new(file, rank)];
			if (piece == CHESS_PIECE_NONE) {
				unsigned int count = 1;
				while (file + 1 <= CHESS_FILE_H) {
					piece = position->board[chess_square_new(file + 1, rank)];
					if (piece != CHESS_PIECE_NONE) {
						break;
					}
					count++;
					file++;
				}
				APPEND_TO_FEN("%c", (char)('0' + count));
			} else {
				size_t written = chess_piece_to_algebraic(piece, fen, fen_size);
				if (written < fen_size) {
					if (fen != NULL) {
						fen += written;
					}
					fen_size -= written;
				} else {
					fen_size = 0;
				}
			}
		}
		if (rank != CHESS_RANK_1) {
			APPEND_TO_FEN("/");
		}
	}

	APPEND_TO_FEN(" ");

	APPEND_TO_FEN("%c", position->side_to_move == CHESS_COLOR_WHITE ? 'w' : 'b');

	APPEND_TO_FEN(" ");

	if (position->castling_rights) {
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE) {
			APPEND_TO_FEN("K");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE) {
			APPEND_TO_FEN("Q");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE) {
			APPEND_TO_FEN("k");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE) {
			APPEND_TO_FEN("q");
		}
	} else {
		APPEND_TO_FEN("-");
	}

	APPEND_TO_FEN(" ");

	if (position->en_passant_square != CHESS_SQUARE_NONE) {
		size_t written = chess_square_to_algebraic(position->en_passant_square, fen, fen_size);
		if (written < fen_size) {
			if (fen != NULL) {
				fen += written;
			}
			fen_size -= written;
		} else {
			fen_size = 0;
		}
	} else {
		APPEND_TO_FEN("-");
	}

	APPEND_TO_FEN(" ");

	APPEND_TO_FEN("%u %u", position->half_move_clock, position->full_move_number);

	return total_written;
#undef APPEND_TO_FEN
}
bool chess_position_is_check(const struct chess_position *position) {
	return chess_square_is_attacked(
	    position,
	    position->king_squares[position->side_to_move],
	    chess_color_opposite(position->side_to_move)
	);
}
bool chess_position_is_checkmate(const struct chess_position *position) {
	return chess_position_is_check(position) && chess_moves_generate(position).count == 0;
}
bool chess_position_is_stalemate(const struct chess_position *position) {
	return !chess_position_is_check(position) && chess_moves_generate(position).count == 0;
}

bool chess_move_is_legal(const struct chess_position *position, struct chess_move move) {
	// TODO: implement actual move legality checking
	struct chess_moves moves = chess_moves_generate(position);
	for (size_t i = 0; i < moves.count; i++) {
		if (moves.moves[i].from == move.from && moves.moves[i].to == move.to &&
		    moves.moves[i].promotion_type == move.promotion_type) {
			return true;
		}
	}

	return false;
}
static void chess_move_do_unchecked(struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position));

	enum chess_piece piece              = position->board[move.from];
	enum chess_piece_type type          = chess_piece_type(piece);
	enum chess_piece captured_piece     = position->board[move.to];
	enum chess_piece_type captured_type = chess_piece_type(captured_piece);
	enum chess_color side_to_move       = position->side_to_move;

	position->board[move.to]            = piece;
	position->board[move.from]          = CHESS_PIECE_NONE;

	position->side_to_move              = chess_color_opposite(side_to_move);

	if (captured_type != CHESS_PIECE_TYPE_NONE) {
		position->half_move_clock = 0;
	} else {
		position->half_move_clock++;
	}

	if (side_to_move == CHESS_COLOR_BLACK) {
		position->full_move_number++;
	}

	if (move.promotion_type != CHESS_PIECE_TYPE_NONE) {
		position->board[move.to] = chess_piece_new(position->side_to_move, move.promotion_type);
	}

	position->en_passant_square = CHESS_SQUARE_NONE;
	if (type == CHESS_PIECE_TYPE_PAWN) {
		position->half_move_clock   = 0;

		enum chess_offset direction = side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
		if (move.to - move.from == 2 * direction) {
			position->en_passant_square = (enum chess_square)(move.from + direction);
		} else if (move.to == position->en_passant_square) {
			position->board[move.to - direction] = CHESS_PIECE_NONE;
		}
	}

	if (type == CHESS_PIECE_TYPE_KING) {
		position->king_squares[chess_piece_color(piece)] = move.to;

		if (move.to - move.from == 2 * CHESS_OFFSET_EAST) {
			position->board[move.to + CHESS_OFFSET_WEST] = position->board[move.to + CHESS_OFFSET_EAST];
			position->board[move.to + CHESS_OFFSET_EAST] = CHESS_PIECE_NONE;
		} else if (move.to - move.from == 2 * CHESS_OFFSET_WEST) {
			position->board[move.to + CHESS_OFFSET_EAST]     = position->board[move.to + 2 * CHESS_OFFSET_WEST];
			position->board[move.to + 2 * CHESS_OFFSET_WEST] = CHESS_PIECE_NONE;
		}

		position->castling_rights &=
		    ~(side_to_move == CHESS_COLOR_WHITE
		          ? CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE
		          : CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE);
	}

	if (type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.from) {
			case CHESS_SQUARE_H1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE); break;
			case CHESS_SQUARE_A1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE); break;
			case CHESS_SQUARE_H8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE); break;
			case CHESS_SQUARE_A8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE); break;
			default:;
		}
	}
	if (captured_type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.to) {
			case CHESS_SQUARE_H1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE); break;
			case CHESS_SQUARE_A1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE); break;
			case CHESS_SQUARE_H8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE); break;
			case CHESS_SQUARE_A8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE); break;
			default:;
		}
	}
}
bool chess_move_do(struct chess_position *position, struct chess_move move) {
	assert(position != nullptr);

	if (!chess_move_is_legal(position, move)) {
		return false;
	}

	chess_move_do_unchecked(position, move);

	assert(chess_position_is_valid(position));

	return true;
}

static void chess_moves_add(struct chess_moves *moves, const struct chess_position *position, struct chess_move move) {
	assert(moves != nullptr && position != nullptr);

	struct chess_position position_after_move = *position;
	chess_move_do_unchecked(&position_after_move, move);
	if (!chess_square_is_attacked(
	        position,
	        position_after_move.king_squares[position->side_to_move],
	        chess_color_opposite(position->side_to_move)
	    )) {
		moves->moves[moves->count++] = move;
	}
}
static void chess_moves_generate_pawn_promotions(
    struct chess_moves *moves,
    const struct chess_position *position,
    enum chess_square from,
    enum chess_square to
) {
	assert(moves != nullptr && position != nullptr);

	static const enum chess_piece_type promotion_types[] = {
		CHESS_PIECE_TYPE_KNIGHT,
		CHESS_PIECE_TYPE_BISHOP,
		CHESS_PIECE_TYPE_ROOK,
		CHESS_PIECE_TYPE_QUEEN,
	};
	for (size_t i = 0; i < ARRAY_LENGTH(promotion_types); i++) {
		chess_moves_add(
		    moves,
		    position,
		    (struct chess_move){
		        .from           = from,
		        .to             = to,
		        .promotion_type = promotion_types[i],
		    }
		);
	}
}
static void chess_moves_generate_pawn(
    struct chess_moves *moves,
    const struct chess_position *position,
    enum chess_square from
) {
	assert(moves != nullptr && position != nullptr);

	enum chess_offset direction = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;

	enum chess_rank promotion_rank = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_RANK_8 : CHESS_RANK_1;

	enum chess_square to           = (enum chess_square)(from + direction);
	if (chess_square_is_valid(to) && position->board[to] == CHESS_PIECE_NONE) {
		if (chess_square_rank(to) == promotion_rank) {
			chess_moves_generate_pawn_promotions(moves, position, from, to);
		} else {
			chess_moves_add(moves, position, (struct chess_move){ .from = from, .to = to });

			enum chess_rank start_rank = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_RANK_2 : CHESS_RANK_7;
			if (chess_square_rank(from) == start_rank) {
				to = (enum chess_square)(from + 2 * direction);
				if (position->board[to] == CHESS_PIECE_NONE) {
					chess_moves_add(moves, position, (struct chess_move){ .from = from, .to = to });
				}
			}
		}
	}

	static const enum chess_offset offsets[] = {
		CHESS_OFFSET_EAST,
		CHESS_OFFSET_WEST,
	};
	for (size_t i = 0; i < ARRAY_LENGTH(offsets); i++) {
		to = (enum chess_square)(from + direction + offsets[i]);
		if (chess_square_is_valid(to) && chess_piece_color(position->board[to]) != position->side_to_move) {
			if (position->board[to] != CHESS_PIECE_NONE || to == position->en_passant_square) {
				if (chess_square_rank(to) == promotion_rank) {
					chess_moves_generate_pawn_promotions(moves, position, from, to);
				} else {
					chess_moves_add(moves, position, (struct chess_move){ .from = from, .to = to });
				}
			}
		}
	}
}
static void chess_moves_generate_directions(
    struct chess_moves *moves,
    const struct chess_position *position,
    enum chess_square from,
    const enum chess_offset *directions,
    size_t direction_count
) {
	assert(moves != nullptr && position != nullptr);

	for (size_t i = 0; i < direction_count; i++) {
		enum chess_offset direction = directions[i];
		enum chess_square to        = from;
		while (true) {
			to += direction;
			if (!chess_square_is_valid(to)) {
				break;
			}

			if (position->board[to] == CHESS_PIECE_NONE) {
				chess_moves_add(moves, position, (struct chess_move){ .from = from, .to = to });
			} else {
				if (chess_piece_color(position->board[to]) != position->side_to_move) {
					chess_moves_add(moves, position, (struct chess_move){ .from = from, .to = to });
				}
				break;
			}
		}
	}
}
static void chess_moves_generate_offsets(
    struct chess_moves *moves,
    const struct chess_position *position,
    enum chess_square from,
    const enum chess_offset *offsets,
    size_t offset_count
) {
	assert(moves != nullptr && position != nullptr);

	for (size_t i = 0; i < offset_count; i++) {
		enum chess_square to = (enum chess_square)(from + offsets[i]);
		if (chess_square_is_valid(to) && chess_piece_color(position->board[to]) != position->side_to_move) {
			chess_moves_add(moves, position, (struct chess_move){ .from = from, .to = to });
		}
	}
}
static void chess_moves_generate_king_castlings(struct chess_moves *moves, const struct chess_position *position) {
	assert(moves != nullptr && position != nullptr);

	enum chess_square from = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_SQUARE_E1 : CHESS_SQUARE_E8;
	enum chess_castling_rights king_side_castling_right  = position->side_to_move == CHESS_COLOR_WHITE
	                                                           ? CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE
	                                                           : CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE;
	enum chess_castling_rights queen_side_castling_right = position->side_to_move == CHESS_COLOR_WHITE
	                                                           ? CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE
	                                                           : CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE;

	enum chess_color other_side                          = chess_color_opposite(position->side_to_move);

	if (position->castling_rights & king_side_castling_right) {
		if (position->board[from + CHESS_OFFSET_EAST] == CHESS_PIECE_NONE &&
		    chess_square_is_attacked(position, (enum chess_square)(from + CHESS_OFFSET_EAST), other_side) &&
		    position->board[from + 2 * CHESS_OFFSET_EAST] == CHESS_PIECE_NONE &&
		    chess_square_is_attacked(position, (enum chess_square)(from + 2 * CHESS_OFFSET_EAST), other_side)) {
			chess_moves_add(
			    moves,
			    position,
			    (struct chess_move){
			        .from = from,
			        .to   = (enum chess_square)(from + 2 * CHESS_OFFSET_EAST),
			    }
			);
		}
	} else if (position->castling_rights & queen_side_castling_right) {
		if (position->board[from + CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    chess_square_is_attacked(position, (enum chess_square)(from + CHESS_OFFSET_WEST), other_side) &&
		    position->board[from + 2 * CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    chess_square_is_attacked(position, (enum chess_square)(from + 2 * CHESS_OFFSET_WEST), other_side) &&
		    position->board[from + 3 * CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    chess_square_is_attacked(position, (enum chess_square)(from + 3 * CHESS_OFFSET_WEST), other_side)) {
			chess_moves_add(
			    moves,
			    position,
			    (struct chess_move){
			        .from = from,
			        .to   = (enum chess_square)(from + 2 * CHESS_OFFSET_WEST),
			    }
			);
		}
	}
}
struct chess_moves chess_moves_generate(const struct chess_position *position) {
	assert(chess_position_is_valid(position));

	struct chess_moves moves = { 0 };

	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square from = chess_square_new(file, rank);

			enum chess_piece piece = position->board[from];

			enum chess_color color = chess_piece_color(piece);
			if (color != position->side_to_move) {
				continue;
			}

			enum chess_piece_type type = chess_piece_type(piece);
			switch (type) {
				case CHESS_PIECE_TYPE_PAWN: {
					chess_moves_generate_pawn(&moves, position, from);
				} break;
				case CHESS_PIECE_TYPE_KNIGHT: {
					static const enum chess_offset offsets[] = {
						2 * CHESS_OFFSET_NORTH + CHESS_OFFSET_EAST, 2 * CHESS_OFFSET_NORTH + CHESS_OFFSET_WEST,
						2 * CHESS_OFFSET_EAST + CHESS_OFFSET_NORTH, 2 * CHESS_OFFSET_EAST + CHESS_OFFSET_SOUTH,
						2 * CHESS_OFFSET_SOUTH + CHESS_OFFSET_EAST, 2 * CHESS_OFFSET_SOUTH + CHESS_OFFSET_WEST,
						2 * CHESS_OFFSET_WEST + CHESS_OFFSET_NORTH, 2 * CHESS_OFFSET_WEST + CHESS_OFFSET_SOUTH,
					};
					chess_moves_generate_offsets(&moves, position, from, offsets, ARRAY_LENGTH(offsets));
				} break;
				case CHESS_PIECE_TYPE_BISHOP: {
					static const enum chess_offset directions[] = {
						CHESS_OFFSET_NORTH_EAST,
						CHESS_OFFSET_SOUTH_EAST,
						CHESS_OFFSET_SOUTH_WEST,
						CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_ROOK: {
					static const enum chess_offset directions[] = {
						CHESS_OFFSET_NORTH,
						CHESS_OFFSET_EAST,
						CHESS_OFFSET_SOUTH,
						CHESS_OFFSET_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_QUEEN: {
					static const enum chess_offset directions[] = {
						CHESS_OFFSET_NORTH,      CHESS_OFFSET_EAST,       CHESS_OFFSET_SOUTH,      CHESS_OFFSET_WEST,
						CHESS_OFFSET_NORTH_EAST, CHESS_OFFSET_SOUTH_EAST, CHESS_OFFSET_SOUTH_WEST, CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_KING: {
					static const enum chess_offset offsets[] = {
						CHESS_OFFSET_NORTH,      CHESS_OFFSET_EAST,       CHESS_OFFSET_SOUTH,      CHESS_OFFSET_WEST,
						CHESS_OFFSET_NORTH_EAST, CHESS_OFFSET_SOUTH_EAST, CHESS_OFFSET_SOUTH_WEST, CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_offsets(&moves, position, from, offsets, ARRAY_LENGTH(offsets));
				} break;
				default: assert(false);
			}
		}
	}

	chess_moves_generate_king_castlings(&moves, position);

	return moves;
}
