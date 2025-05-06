#include <chess.h>

#include <assert.h>
#include <ctype.h>

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
			square = chess_square_move(square, (fen[i] - '0') * CHESS_DIRECTION_EAST);
		} else {
			enum chess_piece_optional piece = CHESS_PIECE_OPTIONAL_NONE;
			switch (fen[i]) {
				case 'P': piece = CHESS_PIECE_OPTIONAL_WHITE_PAWN; break;
				case 'N': piece = CHESS_PIECE_OPTIONAL_WHITE_KNIGHT; break;
				case 'B': piece = CHESS_PIECE_OPTIONAL_WHITE_BISHOP; break;
				case 'R': piece = CHESS_PIECE_OPTIONAL_WHITE_ROOK; break;
				case 'Q': piece = CHESS_PIECE_OPTIONAL_WHITE_QUEEN; break;
				case 'p': piece = CHESS_PIECE_OPTIONAL_BLACK_PAWN; break;
				case 'n': piece = CHESS_PIECE_OPTIONAL_BLACK_KNIGHT; break;
				case 'b': piece = CHESS_PIECE_OPTIONAL_BLACK_BISHOP; break;
				case 'r': piece = CHESS_PIECE_OPTIONAL_BLACK_ROOK; break;
				case 'q': piece = CHESS_PIECE_OPTIONAL_BLACK_QUEEN; break;
				default: assert(false);
			}
			position.board[square] = piece;
			square = chess_square_move(square, CHESS_DIRECTION_EAST);
		}
		i++;
	}

	while (isspace(fen[i])) {
		i++;
	}

	position.side_to_move = fen[i] == 'w' ? CHESS_COLOR_WHITE : CHESS_COLOR_BLACK;

	return position;
}
