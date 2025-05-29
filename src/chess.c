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

#define WRITE(function, ...)                                      \
	do {                                                            \
		size_t _written = function(__VA_ARGS__, string, string_size); \
		if (_written < string_size) {                                 \
			if (string != nullptr) {                                    \
				string += _written;                                       \
			}                                                           \
			string_size -= _written;                                    \
		} else {                                                      \
			string_size = 0;                                            \
		}                                                             \
		total_written += _written;                                    \
	} while (false)
#define WRITE_FORMATTED(...)                                   \
	do {                                                         \
		int _written = snprintf(string, string_size, __VA_ARGS__); \
		assert(_written >= 0);                                     \
		if ((size_t)_written < string_size) {                      \
			if (string != nullptr) {                                 \
				string += (size_t)_written;                            \
			}                                                        \
			string_size -= (size_t)_written;                         \
		} else {                                                   \
			string_size = 0;                                         \
		}                                                          \
		total_written += (size_t)_written;                         \
	} while (false)

#define READ(function, ...)                                    \
	do {                                                         \
		size_t _read = function(__VA_ARGS__, &string[total_read]); \
		if (_read == 0) {                                          \
			return 0;                                                \
		}                                                          \
		total_read += _read;                                       \
	} while (false)

void chess_color_debug(enum chess_color color) {
	switch (color) {
		case CHESS_COLOR_NONE:
			printf("CHESS_COLOR_NONE");
			break;
		case CHESS_COLOR_WHITE:
			printf("CHESS_COLOR_WHITE");
			break;
		case CHESS_COLOR_BLACK:
			printf("CHESS_COLOR_BLACK");
			break;
		default:
			printf("(enum chess_color)%d", color);
			break;
	}
}
bool chess_color_is_valid(enum chess_color color) {
	switch (color) {
		case CHESS_COLOR_WHITE:
		case CHESS_COLOR_BLACK: return true;
		default: return false;
	}
}
enum chess_color chess_color_opposite(enum chess_color color) {
	assert(chess_color_is_valid(color) || color == CHESS_COLOR_NONE);

	switch (color) {
		case CHESS_COLOR_WHITE: return CHESS_COLOR_BLACK;
		case CHESS_COLOR_BLACK: return CHESS_COLOR_WHITE;
		default: return CHESS_COLOR_NONE;
	}
}

void chess_piece_type_debug(enum chess_piece_type type) {
	switch (type) {
		case CHESS_PIECE_TYPE_NONE:
			printf("CHESS_PIECE_TYPE_NONE");
			break;
		case CHESS_PIECE_TYPE_PAWN:
			printf("CHESS_PIECE_TYPE_PAWN");
			break;
		case CHESS_PIECE_TYPE_KNIGHT:
			printf("CHESS_PIECE_TYPE_KNIGHT");
			break;
		case CHESS_PIECE_TYPE_BISHOP:
			printf("CHESS_PIECE_TYPE_BISHOP");
			break;
		case CHESS_PIECE_TYPE_ROOK:
			printf("CHESS_PIECE_TYPE_ROOK");
			break;
		case CHESS_PIECE_TYPE_QUEEN:
			printf("CHESS_PIECE_TYPE_QUEEN");
			break;
		case CHESS_PIECE_TYPE_KING:
			printf("CHESS_PIECE_TYPE_KING");
			break;
		default:
			printf("(enum chess_piece_type)%d", type);
			break;
	}
}
bool chess_piece_type_is_valid(enum chess_piece_type type) {
	switch (type) {
		case CHESS_PIECE_TYPE_PAWN:
		case CHESS_PIECE_TYPE_KNIGHT:
		case CHESS_PIECE_TYPE_BISHOP:
		case CHESS_PIECE_TYPE_ROOK:
		case CHESS_PIECE_TYPE_QUEEN:
		case CHESS_PIECE_TYPE_KING: return true;
		default: return false;
	}
}
size_t chess_piece_type_from_algebraic(enum chess_piece_type *type, const char *string) {
	assert(type != nullptr && string != nullptr);

	switch (string[0]) {
		case 'P': *type = CHESS_PIECE_TYPE_PAWN; break;
		case 'N': *type = CHESS_PIECE_TYPE_KNIGHT; break;
		case 'B': *type = CHESS_PIECE_TYPE_BISHOP; break;
		case 'R': *type = CHESS_PIECE_TYPE_ROOK; break;
		case 'Q': *type = CHESS_PIECE_TYPE_QUEEN; break;
		case 'K': *type = CHESS_PIECE_TYPE_KING; break;
		default: return 0;
	}

	return 1;
}
size_t chess_piece_type_to_algebraic(enum chess_piece_type type, char *string, size_t string_size) {
	assert(chess_piece_type_is_valid(type) && (string != nullptr || string_size == 0));

	if (string != nullptr && string_size >= 2) {
		switch (type) {
			case CHESS_PIECE_TYPE_PAWN: string[0] = 'P'; break;
			case CHESS_PIECE_TYPE_KNIGHT: string[0] = 'N'; break;
			case CHESS_PIECE_TYPE_BISHOP: string[0] = 'B'; break;
			case CHESS_PIECE_TYPE_ROOK: string[0] = 'R'; break;
			case CHESS_PIECE_TYPE_QUEEN: string[0] = 'Q'; break;
			case CHESS_PIECE_TYPE_KING: string[0] = 'K'; break;
			default: assert(false);
		}
		string[1] = '\0';
	}

	return 1;
}

void chess_piece_debug(enum chess_piece piece) {
	switch (piece) {
		case CHESS_PIECE_NONE:
			printf("CHESS_PIECE_NONE");
			break;
		case CHESS_PIECE_WHITE_PAWN:
			printf("CHESS_PIECE_WHITE_PAWN");
			break;
		case CHESS_PIECE_WHITE_KNIGHT:
			printf("CHESS_PIECE_WHITE_KNIGHT");
			break;
		case CHESS_PIECE_WHITE_BISHOP:
			printf("CHESS_PIECE_WHITE_BISHOP");
			break;
		case CHESS_PIECE_WHITE_ROOK:
			printf("CHESS_PIECE_WHITE_ROOK");
			break;
		case CHESS_PIECE_WHITE_QUEEN:
			printf("CHESS_PIECE_WHITE_QUEEN");
			break;
		case CHESS_PIECE_WHITE_KING:
			printf("CHESS_PIECE_WHITE_KING");
			break;
		case CHESS_PIECE_BLACK_PAWN:
			printf("CHESS_PIECE_BLACK_PAWN");
			break;
		case CHESS_PIECE_BLACK_KNIGHT:
			printf("CHESS_PIECE_BLACK_KNIGHT");
			break;
		case CHESS_PIECE_BLACK_BISHOP:
			printf("CHESS_PIECE_BLACK_BISHOP");
			break;
		case CHESS_PIECE_BLACK_ROOK:
			printf("CHESS_PIECE_BLACK_ROOK");
			break;
		case CHESS_PIECE_BLACK_QUEEN:
			printf("CHESS_PIECE_BLACK_QUEEN");
			break;
		case CHESS_PIECE_BLACK_KING:
			printf("CHESS_PIECE_BLACK_KING");
			break;
		default:
			printf("(enum chess_piece)%d", piece);
			break;
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
size_t chess_piece_from_algebraic(enum chess_piece *piece, const char *string) {
	assert(piece != nullptr && string != nullptr);

	switch (string[0]) {
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
size_t chess_piece_to_algebraic(enum chess_piece piece, char *string, size_t string_size) {
	assert(chess_piece_is_valid(piece) && (string != nullptr || string_size == 0));

	if (string != nullptr && string_size >= 2) {
		switch (piece) {
			case CHESS_PIECE_WHITE_PAWN: string[0] = 'P'; break;
			case CHESS_PIECE_WHITE_KNIGHT: string[0] = 'N'; break;
			case CHESS_PIECE_WHITE_BISHOP: string[0] = 'B'; break;
			case CHESS_PIECE_WHITE_ROOK: string[0] = 'R'; break;
			case CHESS_PIECE_WHITE_QUEEN: string[0] = 'Q'; break;
			case CHESS_PIECE_WHITE_KING: string[0] = 'K'; break;
			case CHESS_PIECE_BLACK_PAWN: string[0] = 'p'; break;
			case CHESS_PIECE_BLACK_KNIGHT: string[0] = 'n'; break;
			case CHESS_PIECE_BLACK_BISHOP: string[0] = 'b'; break;
			case CHESS_PIECE_BLACK_ROOK: string[0] = 'r'; break;
			case CHESS_PIECE_BLACK_QUEEN: string[0] = 'q'; break;
			case CHESS_PIECE_BLACK_KING: string[0] = 'k'; break;
			default: assert(false);
		}
		string[1] = '\0';
	}

	return 1;
}

void chess_file_debug(enum chess_file file) {
	switch (file) {
		case CHESS_FILE_NONE:
			printf("CHESS_FILE_NONE");
			break;
		case CHESS_FILE_A:
			printf("CHESS_FILE_A");
			break;
		case CHESS_FILE_B:
			printf("CHESS_FILE_B");
			break;
		case CHESS_FILE_C:
			printf("CHESS_FILE_C");
			break;
		case CHESS_FILE_D:
			printf("CHESS_FILE_D");
			break;
		case CHESS_FILE_E:
			printf("CHESS_FILE_E");
			break;
		case CHESS_FILE_F:
			printf("CHESS_FILE_F");
			break;
		case CHESS_FILE_G:
			printf("CHESS_FILE_G");
			break;
		case CHESS_FILE_H:
			printf("CHESS_FILE_H");
			break;
		default:
			printf("(enum chess_file)%d", file);
			break;
	}
}
bool chess_file_is_valid(enum chess_file file) {
	return (file & 0x8U) == 0x8U;
}
size_t chess_file_from_algebraic(enum chess_file *file, const char *string) {
	assert(file != nullptr && string != nullptr);

	if (string[0] < 'a' || 'h' < string[0]) {
		return 0;
	}

	*file = (enum chess_file)(CHESS_FILE_A + (string[0] - 'a'));

	assert(chess_file_is_valid(*file));

	return 1;
}
size_t chess_file_to_algebraic(enum chess_file file, char *string, size_t string_size) {
	assert(chess_file_is_valid(file) && (string != nullptr || string_size == 0));

	if (string != nullptr && string_size >= 2) {
		string[0] = (char)('a' + (file - CHESS_FILE_A));
		string[1] = '\0';
	}

	return 1;
}

void chess_rank_debug(enum chess_rank rank) {
	switch (rank) {
		case CHESS_RANK_NONE:
			printf("CHESS_RANK_NONE");
			break;
		case CHESS_RANK_1:
			printf("CHESS_RANK_1");
			break;
		case CHESS_RANK_2:
			printf("CHESS_RANK_2");
			break;
		case CHESS_RANK_3:
			printf("CHESS_RANK_3");
			break;
		case CHESS_RANK_4:
			printf("CHESS_RANK_4");
			break;
		case CHESS_RANK_5:
			printf("CHESS_RANK_5");
			break;
		case CHESS_RANK_6:
			printf("CHESS_RANK_6");
			break;
		case CHESS_RANK_7:
			printf("CHESS_RANK_7");
			break;
		case CHESS_RANK_8:
			printf("CHESS_RANK_8");
			break;
		default:
			printf("(enum chess_rank)%d", rank);
			break;
	}
}
bool chess_rank_is_valid(enum chess_rank rank) {
	return (rank & 0x8U) == 0x8U;
}
size_t chess_rank_from_algebraic(enum chess_rank *rank, const char *string) {
	assert(rank != nullptr && string != nullptr);

	if (string[0] < '1' || '8' < string[0]) {
		return 0;
	}

	*rank = (enum chess_rank)(CHESS_RANK_1 + (string[0] - '1'));

	assert(chess_rank_is_valid(*rank));

	return 1;
}
size_t chess_rank_to_algebraic(enum chess_rank rank, char *string, size_t string_size) {
	assert(chess_rank_is_valid(rank) && (string != nullptr || string_size == 0));

	if (string != nullptr && string_size >= 2) {
		string[0] = (char)('1' + (rank - CHESS_RANK_1));
		string[1] = '\0';
	}

	return 1;
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

void chess_square_debug(enum chess_square square) {
	if (square == CHESS_SQUARE_NONE) {
		printf("CHESS_SQUARE_NONE");
	} else if (chess_square_is_valid(square)) {
		enum chess_file file = chess_square_file(square);
		enum chess_rank rank = chess_square_rank(square);

		printf("CHESS_SQUARE_%c%c", 'A' + (file - CHESS_FILE_A), '1' + (rank - CHESS_RANK_1));
	} else {
		printf("(enum chess_square)%d", square);
	}
}
bool chess_square_is_valid(enum chess_square square) {
	return (square & 0x88U) == 0x88U;
}
enum chess_color chess_square_color(enum chess_square square) {
	return (chess_square_file(square) + chess_square_rank(square)) % 2U ? CHESS_COLOR_BLACK : CHESS_COLOR_WHITE;
}
size_t chess_square_from_algebraic(enum chess_square *square, const char *string) {
	assert(square != nullptr && string != nullptr);

	size_t total_read    = 0;

	enum chess_file file = CHESS_FILE_NONE;
	READ(chess_file_from_algebraic, &file);

	enum chess_rank rank = CHESS_RANK_NONE;
	READ(chess_rank_from_algebraic, &rank);

	*square = chess_square_new(file, rank);

	assert(chess_square_is_valid(*square));

	return total_read;
}
size_t chess_square_to_algebraic(enum chess_square square, char *string, size_t string_size) {
	assert(chess_square_is_valid(square));

	size_t total_written = 0;

	WRITE(chess_file_to_algebraic, chess_square_file(square));
	WRITE(chess_rank_to_algebraic, chess_square_rank(square));

	return total_written;
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

			int8_t difference                   = (int8_t)(square - attacker_square);
			uint8_t index                       = (uint8_t)(difference + 0x77);

			enum chess_piece_type attacker_type = chess_piece_type(attacker);
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
						static CHESS_CONSTEXPR enum chess_offset directions[256] = {
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

	if (((position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE) &&
	     (position->board[CHESS_SQUARE_E1] != CHESS_PIECE_WHITE_KING ||
	      position->board[CHESS_SQUARE_H1] != CHESS_PIECE_WHITE_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE) &&
	     (position->board[CHESS_SQUARE_E1] != CHESS_PIECE_WHITE_KING ||
	      position->board[CHESS_SQUARE_A1] != CHESS_PIECE_WHITE_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE) &&
	     (position->board[CHESS_SQUARE_E8] != CHESS_PIECE_BLACK_KING ||
	      position->board[CHESS_SQUARE_H8] != CHESS_PIECE_BLACK_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE) &&
	     (position->board[CHESS_SQUARE_E8] != CHESS_PIECE_BLACK_KING ||
	      position->board[CHESS_SQUARE_A8] != CHESS_PIECE_BLACK_ROOK))) {
		return false;
	}

	if (position->en_passant_square != CHESS_SQUARE_NONE &&
	    (!chess_square_is_valid(position->en_passant_square) ||
	             position->board[position->en_passant_square] != CHESS_PIECE_NONE ||
	             position->side_to_move == CHESS_COLOR_WHITE
	         ? position->board[position->en_passant_square + CHESS_OFFSET_SOUTH] != CHESS_PIECE_BLACK_PAWN ||
	               position->board[position->en_passant_square + CHESS_OFFSET_NORTH] != CHESS_PIECE_NONE
	         : position->board[position->en_passant_square + CHESS_OFFSET_NORTH] != CHESS_PIECE_WHITE_PAWN ||
	               position->board[position->en_passant_square + CHESS_OFFSET_SOUTH] != CHESS_PIECE_NONE)) {
		return false;
	}

	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square square = chess_square_new(file, rank);
			enum chess_piece piece   = position->board[square];
			if (piece != CHESS_PIECE_NONE && !chess_piece_is_valid(piece)) {
				return false;
			}

			enum chess_color color     = chess_piece_color(piece);
			enum chess_piece_type type = chess_piece_type(piece);
			if (type == CHESS_PIECE_TYPE_KING && square != position->king_squares[color]) {
				return false;
			}
		}
	}

	return true;
}
struct chess_position chess_position_new(void) {
	return (struct chess_position){
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
		.side_to_move      = CHESS_COLOR_WHITE,
		.castling_rights   = CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE | CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE,
		.en_passant_square = CHESS_SQUARE_NONE,
		.half_move_clock   = 0,
		.full_move_number  = 1,
		.king_squares      = {
        [CHESS_COLOR_WHITE] = CHESS_SQUARE_E1,
        [CHESS_COLOR_BLACK] = CHESS_SQUARE_E8,
    }
	};
}
size_t chess_position_from_fen(struct chess_position *position, const char *string) {
	assert(position != nullptr && string != nullptr);

	size_t total_read = 0;

	while (isspace(string[total_read])) {
		total_read++;
	}

	memset(position->board, CHESS_PIECE_NONE, sizeof(position->board));
	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square square = chess_square_new(file, rank);
			if ('1' <= string[total_read] && string[total_read] <= '8') {
				if (string[total_read] > CHESS_FILE_H - file + '1') {
					return 0;
				}
				file += (string[total_read] - '1');
				total_read++;
			} else {
				READ(chess_piece_from_algebraic, &position->board[square]);

				if (chess_piece_type(position->board[square]) == CHESS_PIECE_TYPE_KING) {
					position->king_squares[chess_piece_color(position->board[square])] = square;
				}
			}
		}
		if (rank != CHESS_RANK_1) {
			if (string[total_read] != '/') {
				return 0;
			}
			total_read++;
		}
	}

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	if (string[total_read] == 'w') {
		position->side_to_move = CHESS_COLOR_WHITE;
	} else if (string[total_read] == 'b') {
		position->side_to_move = CHESS_COLOR_BLACK;
	} else {
		return 0;
	}
	total_read++;

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	position->castling_rights = CHESS_CASTLING_RIGHTS_NONE;
	while (string[total_read] != '\0' && string[total_read] != ' ') {
		switch (string[total_read]) {
			case 'K': position->castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE; break;
			case 'Q': position->castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE; break;
			case 'k': position->castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE; break;
			case 'q': position->castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE; break;
			case '-': break;
			default: return 0;
		}
		total_read++;
	}

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	if (string[total_read] == '-') {
		position->en_passant_square = CHESS_SQUARE_NONE;
		total_read++;
	} else {
		READ(chess_square_from_algebraic, &position->en_passant_square);
	}

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	errno                = 0;
	char *end            = nullptr;
	unsigned long number = strtoul(string + total_read, &end, 10);
	if (end == string + total_read || errno != 0 || number > UINT_MAX) {
		return 0;
	}
	position->half_move_clock = (unsigned int)number;
	total_read                = (size_t)(end - string);

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	errno  = 0;
	end    = nullptr;
	number = strtoul(string + total_read, &end, 10);
	if (end == string + total_read || errno != 0 || number > UINT_MAX) {
		return 0;
	}
	position->full_move_number = (unsigned int)number;
	total_read                 = (size_t)(end - string);

	return total_read;
}
size_t chess_position_to_fen(const struct chess_position *position, char *string, size_t string_size) {
	assert(chess_position_is_valid(position));

	size_t total_written = 0;

	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_piece piece = position->board[chess_square_new(file, rank)];
			if (piece == CHESS_PIECE_NONE) {
				unsigned int count = 1;
				while (file < CHESS_FILE_H) {
					piece = position->board[chess_square_new(file + 1, rank)];
					if (piece != CHESS_PIECE_NONE) {
						break;
					}
					count++;
					file++;
				}
				WRITE_FORMATTED("%c", (char)('0' + count));
			} else {
				WRITE(chess_piece_to_algebraic, piece);
			}
		}
		if (rank != CHESS_RANK_1) {
			WRITE_FORMATTED("/");
		}
	}

	WRITE_FORMATTED(" ");

	WRITE_FORMATTED("%c", position->side_to_move == CHESS_COLOR_WHITE ? 'w' : 'b');

	WRITE_FORMATTED(" ");

	if (position->castling_rights) {
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE) {
			WRITE_FORMATTED("K");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE) {
			WRITE_FORMATTED("Q");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE) {
			WRITE_FORMATTED("k");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE) {
			WRITE_FORMATTED("q");
		}
	} else {
		WRITE_FORMATTED("-");
	}

	WRITE_FORMATTED(" ");

	if (position->en_passant_square != CHESS_SQUARE_NONE) {
		WRITE(chess_square_to_algebraic, position->en_passant_square);
	} else {
		WRITE_FORMATTED("-");
	}

	WRITE_FORMATTED(" ");

	WRITE_FORMATTED("%u %u", position->half_move_clock, position->full_move_number);

	return total_written;
}
bool chess_position_is_king_attacked(const struct chess_position *position, enum chess_color color) {
	assert(chess_position_is_valid(position));

	return chess_square_is_attacked(position, position->king_squares[color], chess_color_opposite(color));
}
bool chess_position_is_check(const struct chess_position *position) {
	assert(chess_position_is_valid(position));

	return chess_position_is_king_attacked(position, position->side_to_move);
}
bool chess_position_is_checkmate(const struct chess_position *position) {
	assert(chess_position_is_valid(position));

	return chess_position_is_check(position) && chess_moves_generate(position).count == 0;
}
bool chess_position_is_stalemate(const struct chess_position *position) {
	assert(chess_position_is_valid(position));

	return !chess_position_is_check(position) && chess_moves_generate(position).count == 0;
}
bool chess_position_is_fifty_move_rule(const struct chess_position *position) {
	assert(chess_position_is_valid(position));

	return position->half_move_clock >= 100 && !chess_position_is_checkmate(position);
}
bool chess_position_is_threefold_repetition(const struct chess_position *position) {
	assert(chess_position_is_valid(position));

	(void)position;

	// TODO: implement threefold repetition detection

	return false;
}
bool chess_position_is_insufficient_material(const struct chess_position *position) {
	assert(chess_position_is_valid(position));

	(void)position;

	// TODO: implement insufficient material detection

	return false;
}

void chess_move_debug(struct chess_move move) {
	printf("(struct chess_move) {\n");

	printf("\t.from = ");
	chess_square_debug(move.from);
	printf(",\n");

	printf("\t.to = ");
	chess_square_debug(move.to);
	printf(",\n");

	printf("\t.promotion_type = ");
	chess_piece_type_debug(move.promotion_type);
	printf(",\n");

	printf("}");
}
bool chess_move_is_valid(struct chess_move move) {
	if (!chess_square_is_valid(move.from) || !chess_square_is_valid(move.to)) {
		return false;
	}

	switch (move.promotion_type) {
		case CHESS_PIECE_TYPE_NONE:
		case CHESS_PIECE_TYPE_KNIGHT:
		case CHESS_PIECE_TYPE_BISHOP:
		case CHESS_PIECE_TYPE_ROOK:
		case CHESS_PIECE_TYPE_QUEEN: return true;
		default: return false;
	}
}
size_t chess_move_from_algebraic(const struct chess_position *position, struct chess_move *move, const char *string) {
	assert(chess_position_is_valid(position) && move != nullptr && string != nullptr);

	size_t total_read = 0;

	while (isspace(string[total_read])) {
		total_read++;
	}

	if (strncmp(&string[total_read], "O-O", 3) == 0) {
		enum chess_square from = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_SQUARE_E1 : CHESS_SQUARE_E8;

		if (chess_piece_type(position->board[from]) != CHESS_PIECE_TYPE_KING) {
			return 0;
		}

		*move = (struct chess_move){ .from = from, .to = (enum chess_square)(from + 2 * CHESS_OFFSET_EAST) };
		total_read += 3;

		return total_read;
	}
	if (strncmp(&string[total_read], "O-O-O", 5) == 0) {
		enum chess_square from = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_SQUARE_E1 : CHESS_SQUARE_E8;

		if (chess_piece_type(position->board[from]) != CHESS_PIECE_TYPE_KING) {
			return 0;
		}

		*move = (struct chess_move){ .from = from, .to = (enum chess_square)(from + 2 * CHESS_OFFSET_WEST) };
		total_read += 5;

		return total_read;
	}

	enum chess_piece_type type = CHESS_PIECE_TYPE_PAWN;
	total_read += chess_piece_type_from_algebraic(&type, &string[total_read]);

	enum chess_file file = CHESS_FILE_NONE;
	enum chess_rank rank = CHESS_RANK_NONE;
	total_read += chess_file_from_algebraic(&file, &string[total_read]);
	total_read += chess_rank_from_algebraic(&rank, &string[total_read]);

	bool is_capture = false;
	if (string[total_read] == 'x') {
		is_capture = true;
		total_read++;
	}

	enum chess_square to = CHESS_SQUARE_NONE;
	size_t read          = chess_square_from_algebraic(&to, &string[total_read]);
	if (read != 0) {
		total_read += read;
	} else if (file != CHESS_FILE_NONE && rank != CHESS_RANK_NONE && !is_capture) {
		to   = chess_square_new(file, rank);
		file = CHESS_FILE_NONE;
		rank = CHESS_RANK_NONE;
	} else {
		return 0;
	}

	enum chess_piece_type promotion_type = CHESS_PIECE_TYPE_NONE;
	if (string[total_read] == '=') {
		total_read++;
		READ(chess_piece_type_from_algebraic, &promotion_type);
	}

	bool is_check     = false;
	bool is_checkmate = false;
	if (string[total_read] == '+') {
		is_check = true;
		total_read++;
	} else if (string[total_read] == '#') {
		is_checkmate = true;
		total_read++;
	}

	if (file != CHESS_FILE_NONE && rank != CHESS_RANK_NONE) {
		enum chess_square from = chess_square_new(file, rank);

		if (type != CHESS_PIECE_TYPE_NONE && chess_piece_type(position->board[from]) != type) {
			return 0;
		}

		*move = (struct chess_move){
			.from           = from,
			.to             = to,
			.promotion_type = promotion_type,
		};

		if (is_capture && !chess_move_is_capture(position, *move)) {
			return 0;
		}

		if (is_check || is_checkmate) {
			struct chess_position position_after_move = *position;
			if (!chess_move_do(&position_after_move, *move)) {
				return 0;
			}

			if (is_check && !chess_position_is_check(&position_after_move)) {
				return 0;
			}

			if (is_checkmate && !chess_position_is_checkmate(&position_after_move)) {
				return 0;
			}
		}

		return total_read;
	}

	struct chess_moves moves = chess_moves_generate(position);
	size_t matches           = 0;
	for (size_t i = 0; i < moves.count; i++) {
		if (moves.moves[i].to != to) {
			continue;
		}

		if (file != CHESS_FILE_NONE && chess_square_file(moves.moves[i].from) != file) {
			continue;
		}
		if (rank != CHESS_RANK_NONE && chess_square_rank(moves.moves[i].from) != rank) {
			continue;
		}

		if (chess_piece_type(position->board[moves.moves[i].from]) != type) {
			continue;
		}

		if (moves.moves[i].promotion_type != promotion_type) {
			continue;
		}

		if (is_capture && !chess_move_is_capture(position, moves.moves[i])) {
			continue;
		}

		if (is_check || is_checkmate) {
			struct chess_position position_after_move = *position;
			if (!chess_move_do(&position_after_move, moves.moves[i])) {
				continue;
			}

			if (is_check && !chess_position_is_check(&position_after_move)) {
				continue;
			}

			if (is_checkmate && !chess_position_is_checkmate(&position_after_move)) {
				continue;
			}
		}

		*move = moves.moves[i];
		matches++;
	}

	if (matches != 1) {
		return 0;
	}

	return total_read;
}
size_t chess_move_to_algebraic(const struct chess_position *position, struct chess_move move, char *string, size_t string_size) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	size_t total_written = 0;

	if (chess_move_is_kingside_castling(position, move)) {
		WRITE_FORMATTED("O-O");
		return total_written;
	}
	if (chess_move_is_queenside_castling(position, move)) {
		WRITE_FORMATTED("O-O-O");
		return total_written;
	}

	enum chess_piece piece     = position->board[move.from];
	enum chess_piece_type type = chess_piece_type(piece);
	enum chess_file file       = chess_square_file(move.from);
	if (type != CHESS_PIECE_TYPE_PAWN) {
		WRITE(chess_piece_type_to_algebraic, type);

		enum chess_rank rank     = chess_square_rank(move.from);

		bool is_ambiguous        = false;
		bool is_file_ambiguous   = false;
		bool is_rank_ambiguous   = false;

		struct chess_moves moves = chess_moves_generate(position);
		for (size_t i = 0; i < moves.count; i++) {
			if (moves.moves[i].to != move.to || moves.moves[i].from == move.from || position->board[moves.moves[i].from] != piece) {
				continue;
			}

			is_ambiguous = true;

			if (chess_square_file(moves.moves[i].from) == file) {
				is_file_ambiguous = true;
			}
			if (chess_square_rank(moves.moves[i].from) == rank) {
				is_rank_ambiguous = true;
			}
		}
		if (is_ambiguous) {
			if (!is_file_ambiguous) {
				WRITE(chess_file_to_algebraic, file);
			} else if (!is_rank_ambiguous) {
				WRITE(chess_rank_to_algebraic, rank);
			} else {
				WRITE(chess_file_to_algebraic, file);
				WRITE(chess_rank_to_algebraic, rank);
			}
		}
	}

	if (chess_move_is_capture(position, move)) {
		if (type == CHESS_PIECE_TYPE_PAWN) {
			WRITE(chess_file_to_algebraic, file);
		}
		WRITE_FORMATTED("x");
	}

	WRITE(chess_square_to_algebraic, move.to);

	if (chess_move_is_promotion(position, move)) {
		WRITE_FORMATTED("=");
		WRITE(chess_piece_type_to_algebraic, move.promotion_type);
	}

	struct chess_position position_after_move = *position;
	if (chess_move_do(&position_after_move, move)) {
		if (chess_position_is_check(&position_after_move)) {
			if (chess_position_is_checkmate(&position_after_move)) {
				WRITE_FORMATTED("#");
			} else {
				WRITE_FORMATTED("+");
			}
		}
	}

	return total_written;
}
bool chess_move_is_legal(const struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

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
bool chess_move_is_promotion(const struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	(void)position;

	return move.promotion_type != CHESS_PIECE_TYPE_NONE;
}
bool chess_move_is_en_passant(const struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	return chess_piece_type(position->board[move.from]) == CHESS_PIECE_TYPE_PAWN &&
	       move.to == position->en_passant_square;
}
bool chess_move_is_capture(const struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	if (position->board[move.to] != CHESS_PIECE_NONE) {
		return true;
	}

	return chess_move_is_en_passant(position, move);
}
bool chess_move_is_kingside_castling(const struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	return chess_piece_type(position->board[move.from]) == CHESS_PIECE_TYPE_KING &&
	       move.to - move.from == 2 * CHESS_OFFSET_EAST;
}
bool chess_move_is_queenside_castling(const struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	return chess_piece_type(position->board[move.from]) == CHESS_PIECE_TYPE_KING && move.to - move.from == 2 * CHESS_OFFSET_WEST;
}
static void chess_move_do_unchecked(struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	enum chess_piece piece              = position->board[move.from];
	enum chess_piece_type type          = chess_piece_type(piece);
	enum chess_piece captured_piece     = position->board[move.to];
	enum chess_piece_type captured_type = chess_piece_type(captured_piece);
	enum chess_color side_to_move       = position->side_to_move;
	enum chess_square en_passant_square = position->en_passant_square;

	position->board[move.to]            = piece;
	position->board[move.from]          = CHESS_PIECE_NONE;

	position->side_to_move              = chess_color_opposite(side_to_move);

	position->en_passant_square         = CHESS_SQUARE_NONE;

	if (captured_type != CHESS_PIECE_TYPE_NONE) {
		position->half_move_clock = 0;
	} else {
		position->half_move_clock++;
	}

	if (side_to_move == CHESS_COLOR_BLACK) {
		position->full_move_number++;
	}

	if (move.promotion_type != CHESS_PIECE_TYPE_NONE) {
		position->board[move.to] = chess_piece_new(side_to_move, move.promotion_type);
	}

	if (type == CHESS_PIECE_TYPE_PAWN) {
		position->half_move_clock   = 0;

		enum chess_offset direction = side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
		if (move.to - move.from == 2 * direction) {
			position->en_passant_square = (enum chess_square)(move.from + direction);
		} else if (move.to == en_passant_square) {
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
		          ? CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE
		          : CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE);
	}

	if (type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.from) {
			case CHESS_SQUARE_H1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE); break;
			case CHESS_SQUARE_A1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE); break;
			case CHESS_SQUARE_H8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE); break;
			case CHESS_SQUARE_A8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE); break;
			default:;
		}
	}
	if (captured_type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.to) {
			case CHESS_SQUARE_H1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE); break;
			case CHESS_SQUARE_A1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE); break;
			case CHESS_SQUARE_H8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE); break;
			case CHESS_SQUARE_A8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE); break;
			default:;
		}
	}
}
bool chess_move_do(struct chess_position *position, struct chess_move move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	if (!chess_move_is_legal(position, move)) {
		return false;
	}

	chess_move_do_unchecked(position, move);

	assert(chess_position_is_valid(position));

	return true;
}

static void chess_moves_add(struct chess_moves *moves, const struct chess_position *position, struct chess_move move) {
	assert(moves != nullptr && chess_position_is_valid(position) && chess_move_is_valid(move));

	struct chess_position position_after_move = *position;
	chess_move_do_unchecked(&position_after_move, move);
	if (!chess_position_is_king_attacked(&position_after_move, position->side_to_move)) {
		assert(moves->count < CHESS_MOVES_MAXIMUM_COUNT);
		moves->moves[moves->count++] = move;
	}
}
static void chess_moves_generate_pawn_promotions(
    struct chess_moves *moves,
    const struct chess_position *position,
    enum chess_square from,
    enum chess_square to
) {
	assert(moves != nullptr && chess_position_is_valid(position) && chess_square_is_valid(from) && chess_square_is_valid(to));

	static CHESS_CONSTEXPR enum chess_piece_type promotion_types[] = {
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
	assert(moves != nullptr && chess_position_is_valid(position) && chess_square_is_valid(from));

	enum chess_offset direction    = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;

	enum chess_rank promotion_rank = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_RANK_8 : CHESS_RANK_1;

	enum chess_square to           = (enum chess_square)(from + direction);
	if (chess_square_is_valid(to) && position->board[to] == CHESS_PIECE_NONE) {
		if (chess_square_rank(to) == promotion_rank) {
			chess_moves_generate_pawn_promotions(moves, position, from, to);
		} else {
			chess_moves_add(
			    moves,
			    position,
			    (struct chess_move){
			        .from           = from,
			        .to             = to,
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);

			enum chess_rank start_rank = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_RANK_2 : CHESS_RANK_7;
			if (chess_square_rank(from) == start_rank) {
				to = (enum chess_square)(from + 2 * direction);
				if (position->board[to] == CHESS_PIECE_NONE) {
					chess_moves_add(
					    moves,
					    position,
					    (struct chess_move){
					        .from           = from,
					        .to             = to,
					        .promotion_type = CHESS_PIECE_TYPE_NONE,
					    }
					);
				}
			}
		}
	}

	static CHESS_CONSTEXPR enum chess_offset offsets[] = {
		CHESS_OFFSET_EAST,
		CHESS_OFFSET_WEST,
	};
	for (size_t i = 0; i < ARRAY_LENGTH(offsets); i++) {
		to = (enum chess_square)(from + direction + offsets[i]);
		if (to == position->en_passant_square) {
			chess_moves_add(
			    moves,
			    position,
			    (struct chess_move){
			        .from           = from,
			        .to             = to,
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);
		} else if (chess_square_is_valid(to) &&
		           chess_piece_color(position->board[to]) == chess_color_opposite(position->side_to_move)) {
			if (chess_square_rank(to) == promotion_rank) {
				chess_moves_generate_pawn_promotions(moves, position, from, to);
			} else {
				chess_moves_add(
				    moves,
				    position,
				    (struct chess_move){
				        .from           = from,
				        .to             = to,
				        .promotion_type = CHESS_PIECE_TYPE_NONE,
				    }
				);
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
	assert(moves != nullptr && chess_position_is_valid(position) && (directions != nullptr || direction_count == 0));

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
					chess_moves_add(
					    moves,
					    position,
					    (struct chess_move){
					        .from           = from,
					        .to             = to,
					        .promotion_type = CHESS_PIECE_TYPE_NONE,
					    }
					);
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
	assert(moves != nullptr && chess_position_is_valid(position) && (offsets != nullptr || offset_count == 0));

	for (size_t i = 0; i < offset_count; i++) {
		enum chess_square to = (enum chess_square)(from + offsets[i]);
		if (chess_square_is_valid(to) && chess_piece_color(position->board[to]) != position->side_to_move) {
			chess_moves_add(
			    moves,
			    position,
			    (struct chess_move){
			        .from           = from,
			        .to             = to,
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);
		}
	}
}
static void chess_moves_generate_castlings(struct chess_moves *moves, const struct chess_position *position) {
	assert(moves != nullptr && chess_position_is_valid(position));

	enum chess_square from                               = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_SQUARE_E1 : CHESS_SQUARE_E8;
	enum chess_castling_rights king_side_castling_right  = position->side_to_move == CHESS_COLOR_WHITE
	                                                           ? CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE
	                                                           : CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE;
	enum chess_castling_rights queen_side_castling_right = position->side_to_move == CHESS_COLOR_WHITE
	                                                           ? CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE
	                                                           : CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE;

	enum chess_color other_side                          = chess_color_opposite(position->side_to_move);

	if (position->castling_rights & king_side_castling_right) {
		if (position->board[from + CHESS_OFFSET_EAST] == CHESS_PIECE_NONE &&
		    position->board[from + 2 * CHESS_OFFSET_EAST] == CHESS_PIECE_NONE &&
		    !chess_square_is_attacked(position, from, other_side) &&
		    !chess_square_is_attacked(position, (enum chess_square)(from + CHESS_OFFSET_EAST), other_side) &&
		    !chess_square_is_attacked(position, (enum chess_square)(from + 2 * CHESS_OFFSET_EAST), other_side)) {
			chess_moves_add(
			    moves,
			    position,
			    (struct chess_move){
			        .from           = from,
			        .to             = (enum chess_square)(from + 2 * CHESS_OFFSET_EAST),
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);
		}
	}
	if (position->castling_rights & queen_side_castling_right) {
		if (position->board[from + CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    position->board[from + 2 * CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    position->board[from + 3 * CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    !chess_square_is_attacked(position, from, other_side) &&
		    !chess_square_is_attacked(position, (enum chess_square)(from + CHESS_OFFSET_WEST), other_side) &&
		    !chess_square_is_attacked(position, (enum chess_square)(from + 2 * CHESS_OFFSET_WEST), other_side)) {
			chess_moves_add(
			    moves,
			    position,
			    (struct chess_move){
			        .from           = from,
			        .to             = (enum chess_square)(from + 2 * CHESS_OFFSET_WEST),
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
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
					static CHESS_CONSTEXPR enum chess_offset offsets[] = {
						2 * CHESS_OFFSET_NORTH + CHESS_OFFSET_EAST,
						2 * CHESS_OFFSET_NORTH + CHESS_OFFSET_WEST,
						2 * CHESS_OFFSET_EAST + CHESS_OFFSET_NORTH,
						2 * CHESS_OFFSET_EAST + CHESS_OFFSET_SOUTH,
						2 * CHESS_OFFSET_SOUTH + CHESS_OFFSET_EAST,
						2 * CHESS_OFFSET_SOUTH + CHESS_OFFSET_WEST,
						2 * CHESS_OFFSET_WEST + CHESS_OFFSET_NORTH,
						2 * CHESS_OFFSET_WEST + CHESS_OFFSET_SOUTH,
					};
					chess_moves_generate_offsets(&moves, position, from, offsets, ARRAY_LENGTH(offsets));
				} break;
				case CHESS_PIECE_TYPE_BISHOP: {
					static CHESS_CONSTEXPR enum chess_offset directions[] = {
						CHESS_OFFSET_NORTH_EAST,
						CHESS_OFFSET_SOUTH_EAST,
						CHESS_OFFSET_SOUTH_WEST,
						CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_ROOK: {
					static CHESS_CONSTEXPR enum chess_offset directions[] = {
						CHESS_OFFSET_NORTH,
						CHESS_OFFSET_EAST,
						CHESS_OFFSET_SOUTH,
						CHESS_OFFSET_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_QUEEN: {
					static CHESS_CONSTEXPR enum chess_offset directions[] = {
						CHESS_OFFSET_NORTH,
						CHESS_OFFSET_EAST,
						CHESS_OFFSET_SOUTH,
						CHESS_OFFSET_WEST,
						CHESS_OFFSET_NORTH_EAST,
						CHESS_OFFSET_SOUTH_EAST,
						CHESS_OFFSET_SOUTH_WEST,
						CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_KING: {
					static CHESS_CONSTEXPR enum chess_offset offsets[] = {
						CHESS_OFFSET_NORTH,
						CHESS_OFFSET_EAST,
						CHESS_OFFSET_SOUTH,
						CHESS_OFFSET_WEST,
						CHESS_OFFSET_NORTH_EAST,
						CHESS_OFFSET_SOUTH_EAST,
						CHESS_OFFSET_SOUTH_WEST,
						CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_offsets(&moves, position, from, offsets, ARRAY_LENGTH(offsets));
				} break;
				default: assert(false);
			}
		}
	}

	chess_moves_generate_castlings(&moves, position);

	return moves;
}

unsigned long chess_perft_recursive(const struct chess_position *position, unsigned int depth) {
	if (depth == 0) {
		return 1;
	}

	struct chess_moves moves = chess_moves_generate(position);
	if (depth == 1) {
		return moves.count;
	}

	unsigned long count = 0;
	for (size_t i = 0; i < moves.count; i++) {
		struct chess_position position_after_move = *position;
		chess_move_do_unchecked(&position_after_move, moves.moves[i]);
		count += chess_perft_recursive(&position_after_move, depth - 1);
	}

	return count;
}
unsigned long chess_perft(unsigned int depth) {
	struct chess_position position = chess_position_new();
	return chess_perft_recursive(&position, depth);
}
