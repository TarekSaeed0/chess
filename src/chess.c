#include <chess.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum chess_color chess_color_opposite(enum chess_color color) {
	switch (color) {
		case CHESS_COLOR_NONE: return CHESS_COLOR_NONE;
		case CHESS_COLOR_WHITE: return CHESS_COLOR_BLACK;
		case CHESS_COLOR_BLACK: return CHESS_COLOR_WHITE;
		default: return CHESS_COLOR_NONE;
	}
}

enum chess_color chess_square_color(enum chess_square square) {
	return (chess_square_file(square) + chess_square_rank(square)) % 2U ? CHESS_COLOR_BLACK
	                                                                    : CHESS_COLOR_WHITE;
}

enum chess_direction : int8_t {
	CHESS_DIRECTION_NORTH      = CHESS_SQUARE_A2 - CHESS_SQUARE_A1,
	CHESS_DIRECTION_EAST       = CHESS_SQUARE_B1 - CHESS_SQUARE_A1,
	CHESS_DIRECTION_SOUTH      = -CHESS_DIRECTION_NORTH,
	CHESS_DIRECTION_WEST       = -CHESS_DIRECTION_EAST,

	CHESS_DIRECTION_NORTH_EAST = CHESS_DIRECTION_NORTH + CHESS_DIRECTION_EAST,
	CHESS_DIRECTION_SOUTH_EAST = CHESS_DIRECTION_SOUTH + CHESS_DIRECTION_EAST,
	CHESS_DIRECTION_SOUTH_WEST = CHESS_DIRECTION_SOUTH + CHESS_DIRECTION_WEST,
	CHESS_DIRECTION_NORTH_WEST = CHESS_DIRECTION_NORTH + CHESS_DIRECTION_WEST,
};

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
	};
}
bool chess_position_from_fen(struct chess_position *position, const char *fen) {
	assert(fen != NULL);

	size_t i = 0; // NOLINT(readability-identifier-length)

	while (isspace(fen[i])) {
		i++;
	}

	memset(position->board, CHESS_PIECE_NONE, sizeof(position->board));
	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square square = chess_square_new(file, rank);
			if ('1' <= fen[i] && fen[i] <= '8') {
				if (fen[i] > CHESS_FILE_H - file + '1') {
					return false;
				}
				file += (fen[i] - '1');
			} else {
				switch (fen[i]) {
					case 'P': position->board[square] = CHESS_PIECE_WHITE_PAWN; break;
					case 'N': position->board[square] = CHESS_PIECE_WHITE_KNIGHT; break;
					case 'B': position->board[square] = CHESS_PIECE_WHITE_BISHOP; break;
					case 'R': position->board[square] = CHESS_PIECE_WHITE_ROOK; break;
					case 'Q': position->board[square] = CHESS_PIECE_WHITE_QUEEN; break;
					case 'K': position->board[square] = CHESS_PIECE_WHITE_KING; break;
					case 'p': position->board[square] = CHESS_PIECE_BLACK_PAWN; break;
					case 'n': position->board[square] = CHESS_PIECE_BLACK_KNIGHT; break;
					case 'b': position->board[square] = CHESS_PIECE_BLACK_BISHOP; break;
					case 'r': position->board[square] = CHESS_PIECE_BLACK_ROOK; break;
					case 'q': position->board[square] = CHESS_PIECE_BLACK_QUEEN; break;
					case 'k': position->board[square] = CHESS_PIECE_BLACK_KING; break;
					default: return false;
				}
			}
			i++;
		}
		if (rank != CHESS_RANK_1) {
			if (fen[i] != '/') {
				return false;
			}
			i++;
		}
	}

	if (!isspace(fen[i])) {
		return false;
	}
	while (isspace(fen[i])) {
		i++;
	}

	if (fen[i] == 'w') {
		position->side_to_move = CHESS_COLOR_WHITE;
	} else if (fen[i] == 'b') {
		position->side_to_move = CHESS_COLOR_BLACK;
	} else {
		return false;
	}
	i++;

	if (!isspace(fen[i])) {
		return false;
	}
	while (isspace(fen[i])) {
		i++;
	}

	position->castling_rights = CHESS_CASTLING_RIGHTS_NONE;
	while (fen[i] != '\0' && fen[i] != ' ') {
		switch (fen[i]) {
			case 'K': position->castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE; break;
			case 'Q': position->castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE; break;
			case 'k': position->castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE; break;
			case 'q': position->castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE; break;
			case '-': break;
			default: return false;
		}
		i++;
	}

	if (!isspace(fen[i])) {
		return false;
	}
	while (isspace(fen[i])) {
		i++;
	}

	if (fen[i] == '-') {
		position->en_passant_square = CHESS_SQUARE_NONE;
		i++;
	} else {
		assert('a' <= fen[i] && fen[i] <= 'h' && '1' <= fen[i + 1] && fen[i + 1] <= '8');
		enum chess_file file        = (enum chess_file)(CHESS_FILE_A + (fen[i] - 'a'));
		enum chess_rank rank        = (enum chess_rank)(CHESS_RANK_1 + (fen[i + 1] - '1'));
		position->en_passant_square = chess_square_new(file, rank);
		i += 2;
	}

	if (!isspace(fen[i])) {
		return false;
	}
	while (isspace(fen[i])) {
		i++;
	}

	errno                = 0;
	char *end            = nullptr;
	unsigned long number = strtoul(fen + i, &end, 10);
	if (end == fen + i || errno != 0 || number > UINT_MAX) {
		return false;
	}
	position->half_move_clock = (unsigned int)number;
	i                         = (size_t)(end - fen);

	if (!isspace(fen[i])) {
		return false;
	}
	while (isspace(fen[i])) {
		i++;
	}

	errno  = 0;
	end    = nullptr;
	number = strtoul(fen + i, &end, 10);
	if (end == fen + i || errno != 0 || number > UINT_MAX) {
		return false;
	}
	position->full_move_number = (unsigned int)number;

	return true;
}
size_t chess_position_to_fen(const struct chess_position *position, char *fen, size_t fen_size) {
	assert(position != nullptr);

	size_t total_written = 0;
#define APPEND_TO_FEN(...)                                                                         \
	do {                                                                                             \
		int written = snprintf(fen, fen_size, __VA_ARGS__);                                            \
		assert(written >= 0);                                                                          \
		if ((size_t)written < fen_size) {                                                              \
			if (fen != NULL) {                                                                           \
				fen += written;                                                                            \
			}                                                                                            \
			fen_size -= (size_t)written;                                                                 \
		} else {                                                                                       \
			fen_size = 0;                                                                                \
		}                                                                                              \
		total_written += (size_t)written;                                                              \
	} while (0)

	for (enum chess_rank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_piece piece = position->board[chess_square_new(file, rank)];
			if (piece == CHESS_PIECE_NONE) {
				unsigned int count = 0;
				while (file <= CHESS_FILE_H) {
					piece = position->board[chess_square_new(file, rank)];
					if (piece != CHESS_PIECE_NONE) {
						break;
					}
					count++;
					file++;
				}
				APPEND_TO_FEN("%c", (char)('0' + count));
			} else {
				switch (piece) {
					case CHESS_PIECE_WHITE_PAWN: APPEND_TO_FEN("P"); break;
					case CHESS_PIECE_WHITE_KNIGHT: APPEND_TO_FEN("N"); break;
					case CHESS_PIECE_WHITE_BISHOP: APPEND_TO_FEN("B"); break;
					case CHESS_PIECE_WHITE_ROOK: APPEND_TO_FEN("R"); break;
					case CHESS_PIECE_WHITE_QUEEN: APPEND_TO_FEN("Q"); break;
					case CHESS_PIECE_WHITE_KING: APPEND_TO_FEN("K"); break;
					case CHESS_PIECE_BLACK_PAWN: APPEND_TO_FEN("p"); break;
					case CHESS_PIECE_BLACK_KNIGHT: APPEND_TO_FEN("n"); break;
					case CHESS_PIECE_BLACK_BISHOP: APPEND_TO_FEN("b"); break;
					case CHESS_PIECE_BLACK_ROOK: APPEND_TO_FEN("r"); break;
					case CHESS_PIECE_BLACK_QUEEN: APPEND_TO_FEN("q"); break;
					case CHESS_PIECE_BLACK_KING: APPEND_TO_FEN("k"); break;
					default: assert(false);
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
		enum chess_file file = chess_square_file(position->en_passant_square);
		enum chess_rank rank = chess_square_rank(position->en_passant_square);
		APPEND_TO_FEN("%c", (char)('a' + (file - CHESS_FILE_A)));
		APPEND_TO_FEN("%c", (char)('1' + (rank - CHESS_RANK_1)));
	} else {
		APPEND_TO_FEN("-");
	}

	APPEND_TO_FEN(" ");

	APPEND_TO_FEN("%u %u", position->half_move_clock, position->full_move_number);

	return total_written;
#undef APPEND_TO_FEN
}

bool chess_move_is_legal(const struct chess_position *position, struct chess_move move) {
	assert(position != nullptr);

	(void)position, (void)move;
	return true;
}
void chess_move_do_unchecked(struct chess_position *position, struct chess_move move) {
	assert(position != nullptr);

	enum chess_piece piece              = position->board[move.from];
	enum chess_piece_type type          = chess_piece_type(piece);
	enum chess_piece captured_piece     = position->board[move.to];
	enum chess_piece_type captured_type = chess_piece_type(captured_piece);
	enum chess_color side_to_move       = position->side_to_move;

	position->board[move.to]            = piece;
	position->board[move.from]          = CHESS_PIECE_NONE;

	position->side_to_move              = chess_color_opposite(side_to_move);

	if (captured_type != CHESS_PIECE_TYPE_NONE) {
		position->half_move_clock++;
	} else {
		position->half_move_clock = 0;
	}

	if (side_to_move == CHESS_COLOR_BLACK) {
		position->full_move_number++;
	}

	if (move.promotion_type != CHESS_PIECE_TYPE_NONE) {
		position->board[move.to] = chess_piece_new(position->side_to_move, move.promotion_type);
	}

	if (type == CHESS_PIECE_TYPE_KING) {
		if (move.to - move.from == 2 * CHESS_DIRECTION_EAST) {
			position->board[move.to + CHESS_DIRECTION_WEST] =
			    position->board[move.to + CHESS_DIRECTION_EAST];
			position->board[move.to + CHESS_DIRECTION_EAST] = CHESS_PIECE_NONE;
		} else if (move.to - move.from == 2 * CHESS_DIRECTION_WEST) {
			position->board[move.to + CHESS_DIRECTION_EAST] =
			    position->board[move.to + 2 * CHESS_DIRECTION_WEST];
			position->board[move.to + 2 * CHESS_DIRECTION_WEST] = CHESS_PIECE_NONE;
		}

		position->castling_rights &=
		    ~(side_to_move == CHESS_COLOR_WHITE
		          ? CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE
		          : CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE);
	}

	if (type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.from) {
			case CHESS_SQUARE_H1:
				position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE);
				break;
			case CHESS_SQUARE_A1:
				position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE);
				break;
			case CHESS_SQUARE_H8:
				position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE);
				break;
			case CHESS_SQUARE_A8:
				position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE);
				break;
			default:;
		}
	}
	if (captured_type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.to) {
			case CHESS_SQUARE_H1:
				position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE);
				break;
			case CHESS_SQUARE_A1:
				position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE);
				break;
			case CHESS_SQUARE_H8:
				position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE);
				break;
			case CHESS_SQUARE_A8:
				position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE);
				break;
			default:;
		}
	}

	position->en_passant_square = CHESS_SQUARE_NONE;
	if (type == CHESS_PIECE_TYPE_PAWN) {
		position->half_move_clock = 0;

		if (move.to - move.from == 2 * CHESS_DIRECTION_NORTH) {
			position->en_passant_square = (enum chess_square)(move.from + CHESS_DIRECTION_NORTH);
		} else if (move.to - move.from == 2 * CHESS_DIRECTION_SOUTH) {
			position->en_passant_square = (enum chess_square)(move.from + CHESS_DIRECTION_SOUTH);
		} else if (captured_piece == CHESS_PIECE_NONE) {
			if (move.to - move.from == CHESS_DIRECTION_NORTH_EAST ||
			    move.to - move.from == CHESS_DIRECTION_NORTH_WEST) {
				position->board[move.to + CHESS_DIRECTION_SOUTH] = CHESS_PIECE_NONE;
			} else if (move.to - move.from == CHESS_DIRECTION_SOUTH_EAST ||
			           move.to - move.from == CHESS_DIRECTION_SOUTH_WEST) {
				position->board[move.to + CHESS_DIRECTION_NORTH] = CHESS_PIECE_NONE;
			}
		}
	}
}
bool chess_move_do(struct chess_position *position, struct chess_move move) {
	assert(position != nullptr);

	if (!chess_move_is_legal(position, move)) {
		return false;
	}

	chess_move_do_unchecked(position, move);

	return true;
}
