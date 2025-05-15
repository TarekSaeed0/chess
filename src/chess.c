#include <chess.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

enum chess_color chess_color_opposite(enum chess_color color) {
	return (enum chess_color)(color ^ 1U);
}

enum chess_color_optional chess_color_optional_wrap(enum chess_color color) {
	return (enum chess_color_optional)(color + 1);
}
enum chess_color chess_color_optional_unwrap(enum chess_color_optional color) {
	assert(color != CHESS_COLOR_OPTIONAL_NONE);

	return (enum chess_color)(color - 1);
}

enum chess_piece_type_optional chess_piece_type_optional_wrap(enum chess_piece_type type) {
	return (enum chess_piece_type_optional)(type + 1);
}
enum chess_piece_type chess_piece_type_optional_unwrap(enum chess_piece_type_optional type) {
	assert(type != CHESS_PIECE_TYPE_OPTIONAL_NONE);

	return (enum chess_piece_type)(type - 1);
}

enum chess_piece chess_piece_new(enum chess_color color, enum chess_piece_type type) {
	return CHESS_PIECE_NEW(color, type);
}
enum chess_color chess_piece_color(enum chess_piece piece) {
	return CHESS_PIECE_COLOR(piece);
}
enum chess_piece_type chess_piece_type(enum chess_piece piece) {
	return CHESS_PIECE_TYPE(piece);
}

enum chess_piece_optional chess_piece_optional_wrap(enum chess_piece piece) {
	return (enum chess_piece_optional)(piece + 1);
}
enum chess_piece chess_piece_optional_unwrap(enum chess_piece_optional piece) {
	assert(piece != CHESS_PIECE_OPTIONAL_NONE);

	return (enum chess_piece)(piece - 1);
}

enum chess_square chess_square_new(enum chess_file file, enum chess_rank rank) {
	return CHESS_SQUARE_NEW(file, rank);
}
enum chess_file chess_square_file(enum chess_square square) {
	return CHESS_SQUARE_FILE(square);
}
enum chess_rank chess_square_rank(enum chess_square square) {
	return CHESS_SQUARE_RANK(square);
}
enum chess_color chess_square_color(enum chess_square square) {
	return (enum chess_color)((CHESS_SQUARE_FILE(square) + CHESS_SQUARE_RANK(square)) % 2);
}

enum chess_square_optional chess_square_optional_wrap(enum chess_square square) {
	return (enum chess_square_optional)(square + 1);
}
enum chess_square chess_square_optional_unwrap(enum chess_square_optional square) {
	assert(square != CHESS_SQUARE_OPTIONAL_NONE);

	return (enum chess_square)(square - 1);
}

enum chess_direction : int8_t {
	CHESS_DIRECTION_NORTH = CHESS_SQUARE_A2 - CHESS_SQUARE_A1,
	CHESS_DIRECTION_EAST = CHESS_SQUARE_B1 - CHESS_SQUARE_A1,
	CHESS_DIRECTION_SOUTH = -CHESS_DIRECTION_NORTH,
	CHESS_DIRECTION_WEST = -CHESS_DIRECTION_EAST,

	CHESS_DIRECTION_NORTH_EAST = CHESS_DIRECTION_NORTH + CHESS_DIRECTION_EAST,
	CHESS_DIRECTION_SOUTH_EAST = CHESS_DIRECTION_SOUTH + CHESS_DIRECTION_EAST,
	CHESS_DIRECTION_SOUTH_WEST = CHESS_DIRECTION_SOUTH + CHESS_DIRECTION_WEST,
	CHESS_DIRECTION_NORTH_WEST = CHESS_DIRECTION_NORTH + CHESS_DIRECTION_WEST,
};
enum chess_square chess_square_move(enum chess_square square, enum chess_direction direction) {
	return (enum chess_square)((int)square + (int)direction);
}

struct chess_position chess_position_new(void) {
	return (struct chess_position) {
		.board = { 
			[CHESS_SQUARE_A8] = CHESS_PIECE_OPTIONAL_BLACK_ROOK,
			[CHESS_SQUARE_B8] = CHESS_PIECE_OPTIONAL_BLACK_KNIGHT,
			[CHESS_SQUARE_C8] = CHESS_PIECE_OPTIONAL_BLACK_BISHOP,
			[CHESS_SQUARE_D8] = CHESS_PIECE_OPTIONAL_BLACK_QUEEN,
			[CHESS_SQUARE_E8] = CHESS_PIECE_OPTIONAL_BLACK_KING,
			[CHESS_SQUARE_F8] = CHESS_PIECE_OPTIONAL_BLACK_BISHOP,
			[CHESS_SQUARE_G8] = CHESS_PIECE_OPTIONAL_BLACK_KNIGHT,
			[CHESS_SQUARE_H8] = CHESS_PIECE_OPTIONAL_BLACK_ROOK,

			[CHESS_SQUARE_A7] = CHESS_PIECE_OPTIONAL_BLACK_PAWN,
			[CHESS_SQUARE_B7] = CHESS_PIECE_OPTIONAL_BLACK_PAWN,
			[CHESS_SQUARE_C7] = CHESS_PIECE_OPTIONAL_BLACK_PAWN,
			[CHESS_SQUARE_D7] = CHESS_PIECE_OPTIONAL_BLACK_PAWN,
			[CHESS_SQUARE_E7] = CHESS_PIECE_OPTIONAL_BLACK_PAWN,
			[CHESS_SQUARE_F7] = CHESS_PIECE_OPTIONAL_BLACK_PAWN,
			[CHESS_SQUARE_G7] = CHESS_PIECE_OPTIONAL_BLACK_PAWN,
			[CHESS_SQUARE_H7] = CHESS_PIECE_OPTIONAL_BLACK_PAWN,

			[CHESS_SQUARE_A2] = CHESS_PIECE_OPTIONAL_WHITE_PAWN,
			[CHESS_SQUARE_B2] = CHESS_PIECE_OPTIONAL_WHITE_PAWN,
			[CHESS_SQUARE_C2] = CHESS_PIECE_OPTIONAL_WHITE_PAWN,
			[CHESS_SQUARE_D2] = CHESS_PIECE_OPTIONAL_WHITE_PAWN,
			[CHESS_SQUARE_E2] = CHESS_PIECE_OPTIONAL_WHITE_PAWN,
			[CHESS_SQUARE_F2] = CHESS_PIECE_OPTIONAL_WHITE_PAWN,
			[CHESS_SQUARE_G2] = CHESS_PIECE_OPTIONAL_WHITE_PAWN,
			[CHESS_SQUARE_H2] = CHESS_PIECE_OPTIONAL_WHITE_PAWN,

			[CHESS_SQUARE_A1] = CHESS_PIECE_OPTIONAL_WHITE_ROOK,
			[CHESS_SQUARE_B1] = CHESS_PIECE_OPTIONAL_WHITE_KNIGHT,
			[CHESS_SQUARE_C1] = CHESS_PIECE_OPTIONAL_WHITE_BISHOP,
			[CHESS_SQUARE_D1] = CHESS_PIECE_OPTIONAL_WHITE_QUEEN,
			[CHESS_SQUARE_E1] = CHESS_PIECE_OPTIONAL_WHITE_KING,
			[CHESS_SQUARE_F1] = CHESS_PIECE_OPTIONAL_WHITE_BISHOP,
			[CHESS_SQUARE_G1] = CHESS_PIECE_OPTIONAL_WHITE_KNIGHT,
			[CHESS_SQUARE_H1] = CHESS_PIECE_OPTIONAL_WHITE_ROOK,
		},
		.side_to_move = CHESS_COLOR_WHITE,
		.castling_rights = 0,
		.en_passant_square = CHESS_SQUARE_OPTIONAL_NONE,
	};
}
struct chess_position chess_postion_from_fen(const char *fen) {
	assert(fen != NULL);

	struct chess_position position = {};

	size_t i = 0; // NOLINT(readability-identifier-length)

	while (isspace(fen[i])) {
		i++;
	}

	enum chess_square square = CHESS_SQUARE_A8;
	while (fen[i] != '\0' && fen[i] != ' ') {
		if (fen[i] == '/') {
			square = chess_square_move(square, CHESS_DIRECTION_SOUTH + 8 * CHESS_DIRECTION_WEST);
		} else if ('1' <= fen[i] && fen[i] <= '8') {
			square = chess_square_move(
				square,
				(enum chess_direction)((fen[i] - '0') * CHESS_DIRECTION_EAST)
			);
		} else {
			enum chess_piece_optional piece = CHESS_PIECE_OPTIONAL_NONE;
			switch (fen[i]) {
				case 'P': piece = CHESS_PIECE_OPTIONAL_WHITE_PAWN; break;
				case 'N': piece = CHESS_PIECE_OPTIONAL_WHITE_KNIGHT; break;
				case 'B': piece = CHESS_PIECE_OPTIONAL_WHITE_BISHOP; break;
				case 'R': piece = CHESS_PIECE_OPTIONAL_WHITE_ROOK; break;
				case 'Q': piece = CHESS_PIECE_OPTIONAL_WHITE_QUEEN; break;
				case 'K': piece = CHESS_PIECE_OPTIONAL_WHITE_KING; break;
				case 'p': piece = CHESS_PIECE_OPTIONAL_BLACK_PAWN; break;
				case 'n': piece = CHESS_PIECE_OPTIONAL_BLACK_KNIGHT; break;
				case 'b': piece = CHESS_PIECE_OPTIONAL_BLACK_BISHOP; break;
				case 'r': piece = CHESS_PIECE_OPTIONAL_BLACK_ROOK; break;
				case 'q': piece = CHESS_PIECE_OPTIONAL_BLACK_QUEEN; break;
				case 'k': piece = CHESS_PIECE_OPTIONAL_BLACK_KING; break;
				default: assert(false);
			}
			position.board[square] = piece;
			square = chess_square_move(square, CHESS_DIRECTION_EAST);
		}
		i++;
	}

	assert(isspace(fen[i]));
	while (isspace(fen[i])) {
		i++;
	}

	assert(fen[i] == 'w' || fen[i] == 'b');
	position.side_to_move = fen[i] == 'w' ? CHESS_COLOR_WHITE : CHESS_COLOR_BLACK;
	i++;

	assert(isspace(fen[i]));
	while (isspace(fen[i])) {
		i++;
	}

	while (fen[i] != '\0' && fen[i] != ' ') {
		switch (fen[i]) {
			case 'K': position.castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE; break;
			case 'Q': position.castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE; break;
			case 'k': position.castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE; break;
			case 'q': position.castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE; break;
			case '-': break;
			default: assert(false);
		}
		i++;
	}

	assert(isspace(fen[i]));
	while (isspace(fen[i])) {
		i++;
	}

	if (fen[i] == '-') {
		i++;
	} else {
		assert('a' <= fen[i] && fen[i] <= 'h' && '1' <= fen[i + 1] && fen[i + 1] <= '8');
		enum chess_file file = (enum chess_file)(fen[i] - 'a');
		enum chess_rank rank = (enum chess_rank)(fen[i + 1] - '1');
		position.en_passant_square = chess_square_optional_wrap(chess_square_new(file, rank));
		i += 2;
	}

	assert(isspace(fen[i]));
	while (isspace(fen[i])) {
		i++;
	}

	errno = 0;
	char *end = nullptr;
	unsigned long number = strtoul(fen + i, &end, 10);
	assert(end != fen + i && errno == 0 && number <= UINT_MAX);
	position.half_move_clock = (unsigned int)number;
	i = (size_t)(end - fen);

	assert(isspace(fen[i]));
	while (isspace(fen[i])) {
		i++;
	}

	errno = 0;
	end = nullptr;
	number = strtoul(fen + i, &end, 10);
	assert(end != fen + i && errno == 0 && number <= UINT_MAX);
	position.full_move_number = (unsigned int)number;
	i = (size_t)(end - fen);

	assert(isspace(fen[i]));
	while (isspace(fen[i])) {
		i++;
	}

	return position;
}
char *chess_postion_fen(struct chess_position position) {
	size_t fen_size = 128;
	char *fen = calloc(1, fen_size);
	assert(fen != nullptr);

	size_t i = 0; // NOLINT(readability-identifier-length)

	for (enum chess_rank rank = CHESS_RANK_1; rank <= CHESS_RANK_8; rank++) {
		for (enum chess_file file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			enum chess_square square = chess_square_new(file, CHESS_RANK_8 - rank);
			enum chess_piece_optional piece = position.board[square];
			if (piece == CHESS_PIECE_OPTIONAL_NONE) {
				unsigned int count = 0;
				while (file <= CHESS_FILE_H) {
					square = chess_square_move(square, CHESS_DIRECTION_EAST);
					piece = position.board[square];
					if (piece != CHESS_PIECE_OPTIONAL_NONE) {
						break;
					}
					count++;
					file++;
				}
				fen[i++] = (char)('0' + count);
			} else {
				switch (chess_piece_optional_unwrap(piece)) {
					case CHESS_PIECE_WHITE_PAWN: fen[i++] = 'P'; break;
					case CHESS_PIECE_WHITE_KNIGHT: fen[i++] = 'N'; break;
					case CHESS_PIECE_WHITE_BISHOP: fen[i++] = 'B'; break;
					case CHESS_PIECE_WHITE_ROOK: fen[i++] = 'R'; break;
					case CHESS_PIECE_WHITE_QUEEN: fen[i++] = 'Q'; break;
					case CHESS_PIECE_WHITE_KING: fen[i++] = 'K'; break;
					case CHESS_PIECE_BLACK_PAWN: fen[i++] = 'p'; break;
					case CHESS_PIECE_BLACK_KNIGHT: fen[i++] = 'n'; break;
					case CHESS_PIECE_BLACK_BISHOP: fen[i++] = 'b'; break;
					case CHESS_PIECE_BLACK_ROOK: fen[i++] = 'r'; break;
					case CHESS_PIECE_BLACK_QUEEN: fen[i++] = 'q'; break;
					case CHESS_PIECE_BLACK_KING: fen[i++] = 'k'; break;
					default: assert(false);
				}
			}
		}
		if (rank != CHESS_RANK_8) {
			fen[i++] = '/';
		}
	}

	fen[i++] = ' ';

	fen[i++] = position.side_to_move == CHESS_COLOR_WHITE ? 'w' : 'b';

	fen[i++] = ' ';

	if (position.castling_rights) {
		if (position.castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KING_SIDE) {
			fen[i++] = 'K';
		}
		if (position.castling_rights & CHESS_CASTLING_RIGHTS_WHITE_QUEEN_SIDE) {
			fen[i++] = 'Q';
		}
		if (position.castling_rights & CHESS_CASTLING_RIGHTS_BLACK_KING_SIDE) {
			fen[i++] = 'k';
		}
		if (position.castling_rights & CHESS_CASTLING_RIGHTS_BLACK_QUEEN_SIDE) {
			fen[i++] = 'q';
		}
	} else {
		fen[i++] = '-';
	}

	fen[i++] = ' ';

	if (position.en_passant_square != CHESS_SQUARE_OPTIONAL_NONE) {
		enum chess_square square = chess_square_optional_unwrap(position.en_passant_square);
		enum chess_file file = chess_square_file(square);
		enum chess_rank rank = chess_square_rank(square);
		fen[i++] = (char)('a' + file);
		fen[i++] = (char)('1' + rank);

	} else {
		fen[i++] = '-';
	}

	fen[i++] = ' ';

	(void
	)snprintf(&fen[i], fen_size - i, "%u %u", position.half_move_clock, position.full_move_number);

	return fen;
}

void chess_move_do_unchecked(struct chess_position *position, struct chess_move move) {
	assert(position != nullptr);

	position->board[move.to] = position->board[move.from];
	position->board[move.from] = CHESS_PIECE_OPTIONAL_NONE;

	if (move.promotion_type != CHESS_PIECE_TYPE_OPTIONAL_NONE) {
		enum chess_piece_type type = chess_piece_type_optional_unwrap(move.promotion_type);
		enum chess_piece piece = chess_piece_new(position->side_to_move, type);
		position->board[move.to] = chess_piece_optional_wrap(piece);
		return;
	}

	enum chess_piece piece = chess_piece_optional_unwrap(position->board[move.to]);
	enum chess_piece_type type = chess_piece_type(piece);
	if (type == CHESS_PIECE_TYPE_KING) {}
}
bool chess_move_do(struct chess_position *position, struct chess_move move) {
	assert(position != nullptr);

	if (!chess_move_is_legal(*position, move)) {
		return false;
	}

	chess_move_do_unchecked(position, move);

	return true;
}
