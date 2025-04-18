#ifndef CHESS_H
#define CHESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

enum chess_color : uint8_t {
	CHESS_COLOR_WHITE,
	CHESS_COLOR_BLACK,
};
enum chess_color chess_color_opposite(enum chess_color color);

enum chess_color_optional : uint8_t {
	CHESS_COLOR_OPTIONAL_NONE = 0,
	CHESS_COLOR_OPTIONAL_WHITE = CHESS_COLOR_WHITE + 1,
	CHESS_COLOR_OPTIONAL_BLACK = CHESS_COLOR_BLACK + 1,
};
enum chess_color_optional chess_color_optional_wrap(enum chess_color color);
enum chess_color chess_color_optional_unwrap(enum chess_color_optional color);

enum chess_piece_type : uint8_t {
	CHESS_PIECE_TYPE_PAWN,
	CHESS_PIECE_TYPE_KNIGHT,
	CHESS_PIECE_TYPE_BISHOP,
	CHESS_PIECE_TYPE_ROOK,
	CHESS_PIECE_TYPE_QUEEN,
	CHESS_PIECE_TYPE_KING,
};

enum chess_piece_type_optional : uint8_t {
	CHESS_PIECE_TYPE_OPTIONAL_NONE = 0,

	CHESS_PIECE_TYPE_OPTIONAL_PAWN = CHESS_PIECE_TYPE_PAWN + 1,
	CHESS_PIECE_TYPE_OPTIONAL_KNIGHT = CHESS_PIECE_TYPE_KNIGHT + 1,
	CHESS_PIECE_TYPE_OPTIONAL_BISHOP = CHESS_PIECE_TYPE_BISHOP + 1,
	CHESS_PIECE_TYPE_OPTIONAL_ROOK = CHESS_PIECE_TYPE_ROOK + 1,
	CHESS_PIECE_TYPE_OPTIONAL_QUEEN = CHESS_PIECE_TYPE_QUEEN + 1,
	CHESS_PIECE_TYPE_OPTIONAL_KING = CHESS_PIECE_TYPE_KING + 1,
};
enum chess_piece_type_optional chess_piece_type_optional_wrap(enum chess_piece_type type);
enum chess_piece_type chess_piece_type_optional_unwrap(enum chess_piece_type_optional type);

#define CHESS_PIECE_NEW(color, type) ((uint8_t)((color) << 3U) | (type))
enum chess_piece : uint8_t {
	CHESS_PIECE_WHITE_PAWN = CHESS_PIECE_NEW(CHESS_COLOR_WHITE, CHESS_PIECE_TYPE_PAWN),
	CHESS_PIECE_WHITE_KNIGHT = CHESS_PIECE_NEW(CHESS_COLOR_WHITE, CHESS_PIECE_TYPE_KNIGHT),
	CHESS_PIECE_WHITE_BISHOP = CHESS_PIECE_NEW(CHESS_COLOR_WHITE, CHESS_PIECE_TYPE_BISHOP),
	CHESS_PIECE_WHITE_ROOK = CHESS_PIECE_NEW(CHESS_COLOR_WHITE, CHESS_PIECE_TYPE_ROOK),
	CHESS_PIECE_WHITE_QUEEN = CHESS_PIECE_NEW(CHESS_COLOR_WHITE, CHESS_PIECE_TYPE_QUEEN),
	CHESS_PIECE_WHITE_KING = CHESS_PIECE_NEW(CHESS_COLOR_WHITE, CHESS_PIECE_TYPE_KING),

	CHESS_PIECE_BLACK_PAWN = CHESS_PIECE_NEW(CHESS_COLOR_BLACK, CHESS_PIECE_TYPE_PAWN),
	CHESS_PIECE_BLACK_KNIGHT = CHESS_PIECE_NEW(CHESS_COLOR_BLACK, CHESS_PIECE_TYPE_KNIGHT),
	CHESS_PIECE_BLACK_BISHOP = CHESS_PIECE_NEW(CHESS_COLOR_BLACK, CHESS_PIECE_TYPE_BISHOP),
	CHESS_PIECE_BLACK_ROOK = CHESS_PIECE_NEW(CHESS_COLOR_BLACK, CHESS_PIECE_TYPE_ROOK),
	CHESS_PIECE_BLACK_QUEEN = CHESS_PIECE_NEW(CHESS_COLOR_BLACK, CHESS_PIECE_TYPE_QUEEN),
	CHESS_PIECE_BLACK_KING = CHESS_PIECE_NEW(CHESS_COLOR_BLACK, CHESS_PIECE_TYPE_KING),
};
enum chess_piece chess_piece_new(enum chess_color color, enum chess_piece_type type);
enum chess_color chess_piece_color(enum chess_piece piece);
enum chess_piece_type chess_piece_type(enum chess_piece piece);

enum chess_piece_optional : uint8_t {
	CHESS_PIECE_OPTIONAL_NONE = 0,

	CHESS_PIECE_OPTIONAL_WHITE_PAWN = CHESS_PIECE_WHITE_PAWN + 1,
	CHESS_PIECE_OPTIONAL_WHITE_KNIGHT = CHESS_PIECE_WHITE_KNIGHT + 1,
	CHESS_PIECE_OPTIONAL_WHITE_BISHOP = CHESS_PIECE_WHITE_BISHOP + 1,
	CHESS_PIECE_OPTIONAL_WHITE_ROOK = CHESS_PIECE_WHITE_ROOK + 1,
	CHESS_PIECE_OPTIONAL_WHITE_QUEEN = CHESS_PIECE_WHITE_QUEEN + 1,
	CHESS_PIECE_OPTIONAL_WHITE_KING = CHESS_PIECE_WHITE_KING + 1,

	CHESS_PIECE_OPTIONAL_BLACK_PAWN = CHESS_PIECE_BLACK_PAWN + 1,
	CHESS_PIECE_OPTIONAL_BLACK_KNIGHT = CHESS_PIECE_BLACK_KNIGHT + 1,
	CHESS_PIECE_OPTIONAL_BLACK_BISHOP = CHESS_PIECE_BLACK_BISHOP + 1,
	CHESS_PIECE_OPTIONAL_BLACK_ROOK = CHESS_PIECE_BLACK_ROOK + 1,
	CHESS_PIECE_OPTIONAL_BLACK_QUEEN = CHESS_PIECE_BLACK_QUEEN + 1,
	CHESS_PIECE_OPTIONAL_BLACK_KING = CHESS_PIECE_BLACK_KING + 1,
};
enum chess_piece_optional chess_piece_optional_wrap(enum chess_piece piece);
enum chess_piece chess_piece_optional_unwrap(enum chess_piece_optional piece);

enum chess_file : uint8_t {
	CHESS_FILE_A,
	CHESS_FILE_B,
	CHESS_FILE_C,
	CHESS_FILE_D,
	CHESS_FILE_E,
	CHESS_FILE_F,
	CHESS_FILE_G,
	CHESS_FILE_H,
};

enum chess_rank : uint8_t {
	CHESS_RANK_1,
	CHESS_RANK_2,
	CHESS_RANK_3,
	CHESS_RANK_4,
	CHESS_RANK_5,
	CHESS_RANK_6,
	CHESS_RANK_7,
	CHESS_RANK_8,
};

#define CHESS_SQUARE_NEW(file, rank) ((uint8_t)((rank) << 4U) | (file))
enum chess_square : uint8_t {
	CHESS_SQUARE_A1 = CHESS_SQUARE_NEW(CHESS_FILE_A, CHESS_RANK_1),
	CHESS_SQUARE_B1 = CHESS_SQUARE_NEW(CHESS_FILE_B, CHESS_RANK_1),
	CHESS_SQUARE_C1 = CHESS_SQUARE_NEW(CHESS_FILE_C, CHESS_RANK_1),
	CHESS_SQUARE_D1 = CHESS_SQUARE_NEW(CHESS_FILE_D, CHESS_RANK_1),
	CHESS_SQUARE_E1 = CHESS_SQUARE_NEW(CHESS_FILE_E, CHESS_RANK_1),
	CHESS_SQUARE_F1 = CHESS_SQUARE_NEW(CHESS_FILE_F, CHESS_RANK_1),
	CHESS_SQUARE_G1 = CHESS_SQUARE_NEW(CHESS_FILE_G, CHESS_RANK_1),
	CHESS_SQUARE_H1 = CHESS_SQUARE_NEW(CHESS_FILE_H, CHESS_RANK_1),

	CHESS_SQUARE_A2 = CHESS_SQUARE_NEW(CHESS_FILE_A, CHESS_RANK_2),
	CHESS_SQUARE_B2 = CHESS_SQUARE_NEW(CHESS_FILE_B, CHESS_RANK_2),
	CHESS_SQUARE_C2 = CHESS_SQUARE_NEW(CHESS_FILE_C, CHESS_RANK_2),
	CHESS_SQUARE_D2 = CHESS_SQUARE_NEW(CHESS_FILE_D, CHESS_RANK_2),
	CHESS_SQUARE_E2 = CHESS_SQUARE_NEW(CHESS_FILE_E, CHESS_RANK_2),
	CHESS_SQUARE_F2 = CHESS_SQUARE_NEW(CHESS_FILE_F, CHESS_RANK_2),
	CHESS_SQUARE_G2 = CHESS_SQUARE_NEW(CHESS_FILE_G, CHESS_RANK_2),
	CHESS_SQUARE_H2 = CHESS_SQUARE_NEW(CHESS_FILE_H, CHESS_RANK_2),

	CHESS_SQUARE_A3 = CHESS_SQUARE_NEW(CHESS_FILE_A, CHESS_RANK_3),
	CHESS_SQUARE_B3 = CHESS_SQUARE_NEW(CHESS_FILE_B, CHESS_RANK_3),
	CHESS_SQUARE_C3 = CHESS_SQUARE_NEW(CHESS_FILE_C, CHESS_RANK_3),
	CHESS_SQUARE_D3 = CHESS_SQUARE_NEW(CHESS_FILE_D, CHESS_RANK_3),
	CHESS_SQUARE_E3 = CHESS_SQUARE_NEW(CHESS_FILE_E, CHESS_RANK_3),
	CHESS_SQUARE_F3 = CHESS_SQUARE_NEW(CHESS_FILE_F, CHESS_RANK_3),
	CHESS_SQUARE_G3 = CHESS_SQUARE_NEW(CHESS_FILE_G, CHESS_RANK_3),
	CHESS_SQUARE_H3 = CHESS_SQUARE_NEW(CHESS_FILE_H, CHESS_RANK_3),

	CHESS_SQUARE_A4 = CHESS_SQUARE_NEW(CHESS_FILE_A, CHESS_RANK_4),
	CHESS_SQUARE_B4 = CHESS_SQUARE_NEW(CHESS_FILE_B, CHESS_RANK_4),
	CHESS_SQUARE_C4 = CHESS_SQUARE_NEW(CHESS_FILE_C, CHESS_RANK_4),
	CHESS_SQUARE_D4 = CHESS_SQUARE_NEW(CHESS_FILE_D, CHESS_RANK_4),
	CHESS_SQUARE_E4 = CHESS_SQUARE_NEW(CHESS_FILE_E, CHESS_RANK_4),
	CHESS_SQUARE_F4 = CHESS_SQUARE_NEW(CHESS_FILE_F, CHESS_RANK_4),
	CHESS_SQUARE_G4 = CHESS_SQUARE_NEW(CHESS_FILE_G, CHESS_RANK_4),
	CHESS_SQUARE_H4 = CHESS_SQUARE_NEW(CHESS_FILE_H, CHESS_RANK_4),

	CHESS_SQUARE_A5 = CHESS_SQUARE_NEW(CHESS_FILE_A, CHESS_RANK_5),
	CHESS_SQUARE_B5 = CHESS_SQUARE_NEW(CHESS_FILE_B, CHESS_RANK_5),
	CHESS_SQUARE_C5 = CHESS_SQUARE_NEW(CHESS_FILE_C, CHESS_RANK_5),
	CHESS_SQUARE_D5 = CHESS_SQUARE_NEW(CHESS_FILE_D, CHESS_RANK_5),
	CHESS_SQUARE_E5 = CHESS_SQUARE_NEW(CHESS_FILE_E, CHESS_RANK_5),
	CHESS_SQUARE_F5 = CHESS_SQUARE_NEW(CHESS_FILE_F, CHESS_RANK_5),
	CHESS_SQUARE_G5 = CHESS_SQUARE_NEW(CHESS_FILE_G, CHESS_RANK_5),
	CHESS_SQUARE_H5 = CHESS_SQUARE_NEW(CHESS_FILE_H, CHESS_RANK_5),

	CHESS_SQUARE_A6 = CHESS_SQUARE_NEW(CHESS_FILE_A, CHESS_RANK_6),
	CHESS_SQUARE_B6 = CHESS_SQUARE_NEW(CHESS_FILE_B, CHESS_RANK_6),
	CHESS_SQUARE_C6 = CHESS_SQUARE_NEW(CHESS_FILE_C, CHESS_RANK_6),
	CHESS_SQUARE_D6 = CHESS_SQUARE_NEW(CHESS_FILE_D, CHESS_RANK_6),
	CHESS_SQUARE_E6 = CHESS_SQUARE_NEW(CHESS_FILE_E, CHESS_RANK_6),
	CHESS_SQUARE_F6 = CHESS_SQUARE_NEW(CHESS_FILE_F, CHESS_RANK_6),
	CHESS_SQUARE_G6 = CHESS_SQUARE_NEW(CHESS_FILE_G, CHESS_RANK_6),
	CHESS_SQUARE_H6 = CHESS_SQUARE_NEW(CHESS_FILE_H, CHESS_RANK_6),

	CHESS_SQUARE_A7 = CHESS_SQUARE_NEW(CHESS_FILE_A, CHESS_RANK_7),
	CHESS_SQUARE_B7 = CHESS_SQUARE_NEW(CHESS_FILE_B, CHESS_RANK_7),
	CHESS_SQUARE_C7 = CHESS_SQUARE_NEW(CHESS_FILE_C, CHESS_RANK_7),
	CHESS_SQUARE_D7 = CHESS_SQUARE_NEW(CHESS_FILE_D, CHESS_RANK_7),
	CHESS_SQUARE_E7 = CHESS_SQUARE_NEW(CHESS_FILE_E, CHESS_RANK_7),
	CHESS_SQUARE_F7 = CHESS_SQUARE_NEW(CHESS_FILE_F, CHESS_RANK_7),
	CHESS_SQUARE_G7 = CHESS_SQUARE_NEW(CHESS_FILE_G, CHESS_RANK_7),
	CHESS_SQUARE_H7 = CHESS_SQUARE_NEW(CHESS_FILE_H, CHESS_RANK_7),

	CHESS_SQUARE_A8 = CHESS_SQUARE_NEW(CHESS_FILE_A, CHESS_RANK_8),
	CHESS_SQUARE_B8 = CHESS_SQUARE_NEW(CHESS_FILE_B, CHESS_RANK_8),
	CHESS_SQUARE_C8 = CHESS_SQUARE_NEW(CHESS_FILE_C, CHESS_RANK_8),
	CHESS_SQUARE_D8 = CHESS_SQUARE_NEW(CHESS_FILE_D, CHESS_RANK_8),
	CHESS_SQUARE_E8 = CHESS_SQUARE_NEW(CHESS_FILE_E, CHESS_RANK_8),
	CHESS_SQUARE_F8 = CHESS_SQUARE_NEW(CHESS_FILE_F, CHESS_RANK_8),
	CHESS_SQUARE_G8 = CHESS_SQUARE_NEW(CHESS_FILE_G, CHESS_RANK_8),
	CHESS_SQUARE_H8 = CHESS_SQUARE_NEW(CHESS_FILE_H, CHESS_RANK_8),
};
enum chess_square chess_square_new(enum chess_file file, enum chess_rank rank);
enum chess_file chess_square_file(enum chess_square square);
enum chess_rank chess_square_rank(enum chess_square square);
enum chess_color chess_square_color(enum chess_square square);

enum chess_square_optional : uint8_t {
	CHESS_SQUARE_OPTIONAL_NONE = 0,

	CHESS_SQUARE_OPTIONAL_A1 = CHESS_SQUARE_A1 + 1,
	CHESS_SQUARE_OPTIONAL_B1 = CHESS_SQUARE_B1 + 1,
	CHESS_SQUARE_OPTIONAL_C1 = CHESS_SQUARE_C1 + 1,
	CHESS_SQUARE_OPTIONAL_D1 = CHESS_SQUARE_D1 + 1,
	CHESS_SQUARE_OPTIONAL_E1 = CHESS_SQUARE_E1 + 1,
	CHESS_SQUARE_OPTIONAL_F1 = CHESS_SQUARE_F1 + 1,
	CHESS_SQUARE_OPTIONAL_G1 = CHESS_SQUARE_G1 + 1,
	CHESS_SQUARE_OPTIONAL_H1 = CHESS_SQUARE_H1 + 1,

	CHESS_SQUARE_OPTIONAL_A2 = CHESS_SQUARE_A2 + 1,
	CHESS_SQUARE_OPTIONAL_B2 = CHESS_SQUARE_B2 + 1,
	CHESS_SQUARE_OPTIONAL_C2 = CHESS_SQUARE_C2 + 1,
	CHESS_SQUARE_OPTIONAL_D2 = CHESS_SQUARE_D2 + 1,
	CHESS_SQUARE_OPTIONAL_E2 = CHESS_SQUARE_E2 + 1,
	CHESS_SQUARE_OPTIONAL_F2 = CHESS_SQUARE_F2 + 1,
	CHESS_SQUARE_OPTIONAL_G2 = CHESS_SQUARE_G2 + 1,
	CHESS_SQUARE_OPTIONAL_H2 = CHESS_SQUARE_H2 + 1,

	CHESS_SQUARE_OPTIONAL_A3 = CHESS_SQUARE_A3 + 1,
	CHESS_SQUARE_OPTIONAL_B3 = CHESS_SQUARE_B3 + 1,
	CHESS_SQUARE_OPTIONAL_C3 = CHESS_SQUARE_C3 + 1,
	CHESS_SQUARE_OPTIONAL_D3 = CHESS_SQUARE_D3 + 1,
	CHESS_SQUARE_OPTIONAL_E3 = CHESS_SQUARE_E3 + 1,
	CHESS_SQUARE_OPTIONAL_F3 = CHESS_SQUARE_F3 + 1,
	CHESS_SQUARE_OPTIONAL_G3 = CHESS_SQUARE_G3 + 1,
	CHESS_SQUARE_OPTIONAL_H3 = CHESS_SQUARE_H3 + 1,

	CHESS_SQUARE_OPTIONAL_A4 = CHESS_SQUARE_A4 + 1,
	CHESS_SQUARE_OPTIONAL_B4 = CHESS_SQUARE_B4 + 1,
	CHESS_SQUARE_OPTIONAL_C4 = CHESS_SQUARE_C4 + 1,
	CHESS_SQUARE_OPTIONAL_D4 = CHESS_SQUARE_D4 + 1,
	CHESS_SQUARE_OPTIONAL_E4 = CHESS_SQUARE_E4 + 1,
	CHESS_SQUARE_OPTIONAL_F4 = CHESS_SQUARE_F4 + 1,
	CHESS_SQUARE_OPTIONAL_G4 = CHESS_SQUARE_G4 + 1,
	CHESS_SQUARE_OPTIONAL_H4 = CHESS_SQUARE_H4 + 1,

	CHESS_SQUARE_OPTIONAL_A5 = CHESS_SQUARE_A5 + 1,
	CHESS_SQUARE_OPTIONAL_B5 = CHESS_SQUARE_B5 + 1,
	CHESS_SQUARE_OPTIONAL_C5 = CHESS_SQUARE_C5 + 1,
	CHESS_SQUARE_OPTIONAL_D5 = CHESS_SQUARE_D5 + 1,
	CHESS_SQUARE_OPTIONAL_E5 = CHESS_SQUARE_E5 + 1,
	CHESS_SQUARE_OPTIONAL_F5 = CHESS_SQUARE_F5 + 1,
	CHESS_SQUARE_OPTIONAL_G5 = CHESS_SQUARE_G5 + 1,
	CHESS_SQUARE_OPTIONAL_H5 = CHESS_SQUARE_H5 + 1,

	CHESS_SQUARE_OPTIONAL_A6 = CHESS_SQUARE_A6 + 1,
	CHESS_SQUARE_OPTIONAL_B6 = CHESS_SQUARE_B6 + 1,
	CHESS_SQUARE_OPTIONAL_C6 = CHESS_SQUARE_C6 + 1,
	CHESS_SQUARE_OPTIONAL_D6 = CHESS_SQUARE_D6 + 1,
	CHESS_SQUARE_OPTIONAL_E6 = CHESS_SQUARE_E6 + 1,
	CHESS_SQUARE_OPTIONAL_F6 = CHESS_SQUARE_F6 + 1,
	CHESS_SQUARE_OPTIONAL_G6 = CHESS_SQUARE_G6 + 1,
	CHESS_SQUARE_OPTIONAL_H6 = CHESS_SQUARE_H6 + 1,

	CHESS_SQUARE_OPTIONAL_A7 = CHESS_SQUARE_A7 + 1,
	CHESS_SQUARE_OPTIONAL_B7 = CHESS_SQUARE_B7 + 1,
	CHESS_SQUARE_OPTIONAL_C7 = CHESS_SQUARE_C7 + 1,
	CHESS_SQUARE_OPTIONAL_D7 = CHESS_SQUARE_D7 + 1,
	CHESS_SQUARE_OPTIONAL_E7 = CHESS_SQUARE_E7 + 1,
	CHESS_SQUARE_OPTIONAL_F7 = CHESS_SQUARE_F7 + 1,
	CHESS_SQUARE_OPTIONAL_G7 = CHESS_SQUARE_G7 + 1,
	CHESS_SQUARE_OPTIONAL_H7 = CHESS_SQUARE_H7 + 1,

	CHESS_SQUARE_OPTIONAL_A8 = CHESS_SQUARE_A8 + 1,
	CHESS_SQUARE_OPTIONAL_B8 = CHESS_SQUARE_B8 + 1,
	CHESS_SQUARE_OPTIONAL_C8 = CHESS_SQUARE_C8 + 1,
	CHESS_SQUARE_OPTIONAL_D8 = CHESS_SQUARE_D8 + 1,
	CHESS_SQUARE_OPTIONAL_E8 = CHESS_SQUARE_E8 + 1,
	CHESS_SQUARE_OPTIONAL_F8 = CHESS_SQUARE_F8 + 1,
	CHESS_SQUARE_OPTIONAL_G8 = CHESS_SQUARE_G8 + 1,
	CHESS_SQUARE_OPTIONAL_H8 = CHESS_SQUARE_H8 + 1,
};
enum chess_square_optional chess_square_optional_wrap(enum chess_square square);
enum chess_square chess_square_optional_unwrap(enum chess_square_optional square);

struct chess_position {
	enum chess_piece_optional board[128];
	enum chess_color side_to_move;
};
struct chess_position chess_position_new(void);
struct chess_position chess_postion_from_fen(const char *fen);
char *chess_postion_fen(struct chess_position position);

struct chess_move {
	enum chess_square from;
	enum chess_square to;
	enum chess_piece_optional captured_piece;
	enum chess_piece_type_optional promotion_type;
};
bool chess_move_is_legal(struct chess_position position, struct chess_move move);
void chess_move_do(struct chess_position *position, struct chess_move move);
void chess_move_undo(struct chess_position *position, struct chess_move move);

constexpr size_t CHESS_MOVES_MAXIMUM_COUNT = 256;

struct chess_moves {
	struct chess_move moves[CHESS_MOVES_MAXIMUM_COUNT];
	size_t count;
};
struct chess_moves_filter {
	enum chess_square_optional from;
	enum chess_square_optional to;
	enum chess_color_optional color;
};
struct chess_moves chess_moves_generate(
	struct chess_position position,
	struct chess_moves_filter filter
);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H
