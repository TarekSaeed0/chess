#ifndef CHESS_H
#define CHESS_H

#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if (defined(__cplusplus) && __cplusplus >= 201103L) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L)
#define CHESS_HAS_CONSTEXPR 1
#else
#define CHESS_HAS_CONSTEXPR 0
#endif

#if CHESS_HAS_CONSTEXPR
#define CHESS_CONSTEXPR constexpr
#else
#define CHESS_CONSTEXPR const
#endif

#if CHESS_HAS_CONSTEXPR
static constexpr size_t CHESS_MOVES_MAXIMUM_COUNT = 256;
#else
enum {
	CHESS_MOVES_MAXIMUM_COUNT = 256,
};
#endif

enum chess_color : uint8_t {
	CHESS_COLOR_NONE  = 0U,

	CHESS_COLOR_WHITE = 1U,
	CHESS_COLOR_BLACK = 2U,
};

enum chess_piece_type : uint8_t {
	CHESS_PIECE_TYPE_NONE   = 0U,

	CHESS_PIECE_TYPE_PAWN   = 1U,
	CHESS_PIECE_TYPE_KNIGHT = 2U,
	CHESS_PIECE_TYPE_BISHOP = 3U,
	CHESS_PIECE_TYPE_ROOK   = 4U,
	CHESS_PIECE_TYPE_QUEEN  = 5U,
	CHESS_PIECE_TYPE_KING   = 6U,
};

enum chess_piece : uint8_t {
	CHESS_PIECE_NONE         = CHESS_COLOR_NONE << 3U | CHESS_PIECE_TYPE_NONE,

	CHESS_PIECE_WHITE_PAWN   = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_PAWN,
	CHESS_PIECE_WHITE_KNIGHT = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_KNIGHT,
	CHESS_PIECE_WHITE_BISHOP = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_BISHOP,
	CHESS_PIECE_WHITE_ROOK   = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_ROOK,
	CHESS_PIECE_WHITE_QUEEN  = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_QUEEN,
	CHESS_PIECE_WHITE_KING   = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_KING,

	CHESS_PIECE_BLACK_PAWN   = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_PAWN,
	CHESS_PIECE_BLACK_KNIGHT = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_KNIGHT,
	CHESS_PIECE_BLACK_BISHOP = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_BISHOP,
	CHESS_PIECE_BLACK_ROOK   = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_ROOK,
	CHESS_PIECE_BLACK_QUEEN  = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_QUEEN,
	CHESS_PIECE_BLACK_KING   = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_KING,
};

enum chess_file : uint8_t {
	CHESS_FILE_NONE = 0U,

	CHESS_FILE_A    = 8U,
	CHESS_FILE_B    = 9U,
	CHESS_FILE_C    = 10U,
	CHESS_FILE_D    = 11U,
	CHESS_FILE_E    = 12U,
	CHESS_FILE_F    = 13U,
	CHESS_FILE_G    = 14U,
	CHESS_FILE_H    = 15U,
};

enum chess_rank : uint8_t {
	CHESS_RANK_NONE = 0U,

	CHESS_RANK_1    = 8U,
	CHESS_RANK_2    = 9U,
	CHESS_RANK_3    = 10U,
	CHESS_RANK_4    = 11U,
	CHESS_RANK_5    = 12U,
	CHESS_RANK_6    = 13U,
	CHESS_RANK_7    = 14U,
	CHESS_RANK_8    = 15U,
};

enum chess_square : uint8_t {
	CHESS_SQUARE_NONE = CHESS_FILE_NONE | CHESS_RANK_NONE << 4U,

	CHESS_SQUARE_A1   = CHESS_FILE_A | CHESS_RANK_1 << 4U,
	CHESS_SQUARE_B1   = CHESS_FILE_B | CHESS_RANK_1 << 4U,
	CHESS_SQUARE_C1   = CHESS_FILE_C | CHESS_RANK_1 << 4U,
	CHESS_SQUARE_D1   = CHESS_FILE_D | CHESS_RANK_1 << 4U,
	CHESS_SQUARE_E1   = CHESS_FILE_E | CHESS_RANK_1 << 4U,
	CHESS_SQUARE_F1   = CHESS_FILE_F | CHESS_RANK_1 << 4U,
	CHESS_SQUARE_G1   = CHESS_FILE_G | CHESS_RANK_1 << 4U,
	CHESS_SQUARE_H1   = CHESS_FILE_H | CHESS_RANK_1 << 4U,

	CHESS_SQUARE_A2   = CHESS_FILE_A | CHESS_RANK_2 << 4U,
	CHESS_SQUARE_B2   = CHESS_FILE_B | CHESS_RANK_2 << 4U,
	CHESS_SQUARE_C2   = CHESS_FILE_C | CHESS_RANK_2 << 4U,
	CHESS_SQUARE_D2   = CHESS_FILE_D | CHESS_RANK_2 << 4U,
	CHESS_SQUARE_E2   = CHESS_FILE_E | CHESS_RANK_2 << 4U,
	CHESS_SQUARE_F2   = CHESS_FILE_F | CHESS_RANK_2 << 4U,
	CHESS_SQUARE_G2   = CHESS_FILE_G | CHESS_RANK_2 << 4U,
	CHESS_SQUARE_H2   = CHESS_FILE_H | CHESS_RANK_2 << 4U,

	CHESS_SQUARE_A3   = CHESS_FILE_A | CHESS_RANK_3 << 4U,
	CHESS_SQUARE_B3   = CHESS_FILE_B | CHESS_RANK_3 << 4U,
	CHESS_SQUARE_C3   = CHESS_FILE_C | CHESS_RANK_3 << 4U,
	CHESS_SQUARE_D3   = CHESS_FILE_D | CHESS_RANK_3 << 4U,
	CHESS_SQUARE_E3   = CHESS_FILE_E | CHESS_RANK_3 << 4U,
	CHESS_SQUARE_F3   = CHESS_FILE_F | CHESS_RANK_3 << 4U,
	CHESS_SQUARE_G3   = CHESS_FILE_G | CHESS_RANK_3 << 4U,
	CHESS_SQUARE_H3   = CHESS_FILE_H | CHESS_RANK_3 << 4U,

	CHESS_SQUARE_A4   = CHESS_FILE_A | CHESS_RANK_4 << 4U,
	CHESS_SQUARE_B4   = CHESS_FILE_B | CHESS_RANK_4 << 4U,
	CHESS_SQUARE_C4   = CHESS_FILE_C | CHESS_RANK_4 << 4U,
	CHESS_SQUARE_D4   = CHESS_FILE_D | CHESS_RANK_4 << 4U,
	CHESS_SQUARE_E4   = CHESS_FILE_E | CHESS_RANK_4 << 4U,
	CHESS_SQUARE_F4   = CHESS_FILE_F | CHESS_RANK_4 << 4U,
	CHESS_SQUARE_G4   = CHESS_FILE_G | CHESS_RANK_4 << 4U,
	CHESS_SQUARE_H4   = CHESS_FILE_H | CHESS_RANK_4 << 4U,

	CHESS_SQUARE_A5   = CHESS_FILE_A | CHESS_RANK_5 << 4U,
	CHESS_SQUARE_B5   = CHESS_FILE_B | CHESS_RANK_5 << 4U,
	CHESS_SQUARE_C5   = CHESS_FILE_C | CHESS_RANK_5 << 4U,
	CHESS_SQUARE_D5   = CHESS_FILE_D | CHESS_RANK_5 << 4U,
	CHESS_SQUARE_E5   = CHESS_FILE_E | CHESS_RANK_5 << 4U,
	CHESS_SQUARE_F5   = CHESS_FILE_F | CHESS_RANK_5 << 4U,
	CHESS_SQUARE_G5   = CHESS_FILE_G | CHESS_RANK_5 << 4U,
	CHESS_SQUARE_H5   = CHESS_FILE_H | CHESS_RANK_5 << 4U,

	CHESS_SQUARE_A6   = CHESS_FILE_A | CHESS_RANK_6 << 4U,
	CHESS_SQUARE_B6   = CHESS_FILE_B | CHESS_RANK_6 << 4U,
	CHESS_SQUARE_C6   = CHESS_FILE_C | CHESS_RANK_6 << 4U,
	CHESS_SQUARE_D6   = CHESS_FILE_D | CHESS_RANK_6 << 4U,
	CHESS_SQUARE_E6   = CHESS_FILE_E | CHESS_RANK_6 << 4U,
	CHESS_SQUARE_F6   = CHESS_FILE_F | CHESS_RANK_6 << 4U,
	CHESS_SQUARE_G6   = CHESS_FILE_G | CHESS_RANK_6 << 4U,
	CHESS_SQUARE_H6   = CHESS_FILE_H | CHESS_RANK_6 << 4U,

	CHESS_SQUARE_A7   = CHESS_FILE_A | CHESS_RANK_7 << 4U,
	CHESS_SQUARE_B7   = CHESS_FILE_B | CHESS_RANK_7 << 4U,
	CHESS_SQUARE_C7   = CHESS_FILE_C | CHESS_RANK_7 << 4U,
	CHESS_SQUARE_D7   = CHESS_FILE_D | CHESS_RANK_7 << 4U,
	CHESS_SQUARE_E7   = CHESS_FILE_E | CHESS_RANK_7 << 4U,
	CHESS_SQUARE_F7   = CHESS_FILE_F | CHESS_RANK_7 << 4U,
	CHESS_SQUARE_G7   = CHESS_FILE_G | CHESS_RANK_7 << 4U,
	CHESS_SQUARE_H7   = CHESS_FILE_H | CHESS_RANK_7 << 4U,

	CHESS_SQUARE_A8   = CHESS_FILE_A | CHESS_RANK_8 << 4U,
	CHESS_SQUARE_B8   = CHESS_FILE_B | CHESS_RANK_8 << 4U,
	CHESS_SQUARE_C8   = CHESS_FILE_C | CHESS_RANK_8 << 4U,
	CHESS_SQUARE_D8   = CHESS_FILE_D | CHESS_RANK_8 << 4U,
	CHESS_SQUARE_E8   = CHESS_FILE_E | CHESS_RANK_8 << 4U,
	CHESS_SQUARE_F8   = CHESS_FILE_F | CHESS_RANK_8 << 4U,
	CHESS_SQUARE_G8   = CHESS_FILE_G | CHESS_RANK_8 << 4U,
	CHESS_SQUARE_H8   = CHESS_FILE_H | CHESS_RANK_8 << 4U,
};

enum chess_castling_rights : uint8_t {
	CHESS_CASTLING_RIGHTS_NONE            = 0U,

	CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE  = 1U << 0U,
	CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE = 1U << 1U,
	CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE  = 1U << 2U,
	CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE = 1U << 3U,
};

struct chess_position {
	enum chess_piece board[256];
	enum chess_color side_to_move;
	enum chess_castling_rights castling_rights;
	enum chess_square en_passant_square;
	unsigned int half_move_clock;
	unsigned int full_move_number;
	enum chess_square king_squares[3];
};

struct chess_move {
	enum chess_square from;
	enum chess_square to;
	enum chess_piece_type promotion_type;
};

struct chess_moves {
	struct chess_move moves[CHESS_MOVES_MAXIMUM_COUNT];
	size_t count;
};

void chess_color_debug(enum chess_color color);
bool chess_color_is_valid(enum chess_color color);
enum chess_color chess_color_opposite(enum chess_color color);

void chess_piece_type_debug(enum chess_piece_type type);
bool chess_piece_type_is_valid(enum chess_piece_type type);
size_t chess_piece_type_from_algebraic(enum chess_piece_type *type, const char *string);
size_t chess_piece_type_to_algebraic(enum chess_piece_type type, char *string, size_t string_size);

void chess_piece_debug(enum chess_piece piece);
bool chess_piece_is_valid(enum chess_piece piece);
static inline enum chess_piece chess_piece_new(enum chess_color color, enum chess_piece_type type) {
	return (enum chess_piece)(color << 3U | type);
}
static inline enum chess_color chess_piece_color(enum chess_piece piece) {
	return (enum chess_color)(piece >> 3U);
}
static inline enum chess_piece_type chess_piece_type(enum chess_piece piece) {
	return (enum chess_piece_type)(piece & 0x7U);
}
size_t chess_piece_from_algebraic(enum chess_piece *piece, const char *string);
size_t chess_piece_to_algebraic(enum chess_piece piece, char *string, size_t string_size);

void chess_file_debug(enum chess_file file);
bool chess_file_is_valid(enum chess_file file);
size_t chess_file_from_algebraic(enum chess_file *file, const char *string);
size_t chess_file_to_algebraic(enum chess_file file, char *string, size_t string_size);

void chess_rank_debug(enum chess_rank rank);
bool chess_rank_is_valid(enum chess_rank rank);
size_t chess_rank_from_algebraic(enum chess_rank *rank, const char *string);
size_t chess_rank_to_algebraic(enum chess_rank rank, char *string, size_t string_size);

void chess_square_debug(enum chess_square square);
bool chess_square_is_valid(enum chess_square square);
static inline enum chess_square chess_square_new(enum chess_file file, enum chess_rank rank) {
	return (enum chess_square)(file | rank << 4U);
}
static inline enum chess_file chess_square_file(enum chess_square square) {
	return (enum chess_file)(square & 0xFU);
}
static inline enum chess_rank chess_square_rank(enum chess_square square) {
	return (enum chess_rank)(square >> 4U);
}
enum chess_color chess_square_color(enum chess_square square);
size_t chess_square_from_algebraic(enum chess_square *square, const char *string);
size_t chess_square_to_algebraic(enum chess_square square, char *string, size_t string_size);
bool chess_square_is_attacked(const struct chess_position *position, enum chess_square square, enum chess_color color);

bool chess_position_is_valid(const struct chess_position *position);
struct chess_position chess_position_new(void);
size_t chess_position_from_fen(struct chess_position *position, const char *string);
size_t chess_position_to_fen(const struct chess_position *position, char *string, size_t string_size);
bool chess_position_is_check(const struct chess_position *position);
bool chess_position_is_checkmate(const struct chess_position *position);
bool chess_position_is_stalemate(const struct chess_position *position);
bool chess_position_is_fifty_move_rule(const struct chess_position *position);
bool chess_position_is_threefold_repetition(const struct chess_position *position);
bool chess_position_is_insufficient_material(const struct chess_position *position);

void chess_move_debug(struct chess_move move);
bool chess_move_is_valid(struct chess_move move);
size_t chess_move_from_algebraic(const struct chess_position *position, struct chess_move *move, const char *string);
size_t chess_move_to_algebraic(const struct chess_position *position, struct chess_move move, char *string, size_t string_size);
bool chess_move_is_legal(const struct chess_position *position, struct chess_move move);
bool chess_move_is_promotion(const struct chess_position *position, struct chess_move move);
bool chess_move_is_en_passant(const struct chess_position *position, struct chess_move move);
bool chess_move_is_capture(const struct chess_position *position, struct chess_move move);
bool chess_move_is_kingside_castling(const struct chess_position *position, struct chess_move move);
bool chess_move_is_queenside_castling(const struct chess_position *position, struct chess_move move);
bool chess_move_do(struct chess_position *position, struct chess_move move);

struct chess_moves chess_moves_generate(const struct chess_position *position);

unsigned long chess_perft(unsigned int depth);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H
