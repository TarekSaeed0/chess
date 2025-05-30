#ifndef CHESS_H
#define CHESS_H

#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif

#if defined(__cplusplus) && defined(__cpp_constexpr)
	#define CHESS_HAS_CONSTEXPR
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	#if defined(__clang__)
		#if (__clang_major__ > 19) || (__clang_major__ == 19 && __clang_minor__ >= 1)
			#define CHESS_HAS_CONSTEXPR
		#endif
	#else
		#define CHESS_HAS_CONSTEXPR
	#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
	#define CHESS_HAS_TYPED_ENUM
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	#define CHESS_HAS_TYPED_ENUM
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
	#define CHESS_HAS_NULLPTR
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	#define CHESS_HAS_NULLPTR
#endif

#ifdef CHESS_HAS_CONSTEXPR
	#define CHESS_CONSTEXPR constexpr
#else
	#define CHESS_CONSTEXPR const
#endif

#ifdef CHESS_HAS_CONSTEXPR
	#define CHESS_DEFINE_INTEGRAL_CONSTANT(type, name, value) \
		static CHESS_CONSTEXPR typeof(value) name = (value)
#else
	#ifdef CHESS_HAS_TYPED_ENUM
		#define CHESS_DEFINE_INTEGRAL_CONSTANT(type, name, value) \
			enum : type {                                           \
				name = (value)                                        \
			}
	#else
		#define CHESS_DEFINE_INTEGRAL_CONSTANT(type, name, value) \
			enum {                                                  \
				name = (value)                                        \
			}
	#endif
#endif

#ifdef CHESS_HAS_TYPED_ENUM
	#define CHESS_ENUM(type, name) \
		enum name : type;            \
		typedef enum name name;      \
		enum name : type
#else
	#define CHESS_ENUM(type, name) \
		typedef type name;           \
		enum name
#endif

#ifdef CHESS_HAS_NULLPTR
	#define CHESS_NULL nullptr
#else
	#define CHESS_NULL NULL
#endif

CHESS_DEFINE_INTEGRAL_CONSTANT(size_t, CHESS_MOVES_MAXIMUM_COUNT, 256);

CHESS_ENUM(uint8_t, ChessColor) {
	CHESS_COLOR_NONE  = 0U,

	CHESS_COLOR_WHITE = 1U,
	CHESS_COLOR_BLACK = 2U,
};

CHESS_ENUM(uint8_t, ChessPieceType) {
	CHESS_PIECE_TYPE_NONE   = 0U,

	CHESS_PIECE_TYPE_PAWN   = 1U,
	CHESS_PIECE_TYPE_KNIGHT = 2U,
	CHESS_PIECE_TYPE_BISHOP = 3U,
	CHESS_PIECE_TYPE_ROOK   = 4U,
	CHESS_PIECE_TYPE_QUEEN  = 5U,
	CHESS_PIECE_TYPE_KING   = 6U,
};

CHESS_ENUM(uint8_t, ChessPiece) {
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

CHESS_ENUM(uint8_t, ChessFile) {
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

CHESS_ENUM(uint8_t, ChessRank) {
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

CHESS_ENUM(uint8_t, ChessSquare) {
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

CHESS_ENUM(uint8_t, ChessCastlingRights) {
	CHESS_CASTLING_RIGHTS_NONE            = 0U,

	CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE  = 1U << 0U,
	CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE = 1U << 1U,
	CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE  = 1U << 2U,
	CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE = 1U << 3U,
};

typedef struct ChessPosition {
	ChessPiece board[256];
	ChessColor side_to_move;
	ChessCastlingRights castling_rights;
	ChessSquare en_passant_square;
	unsigned int half_move_clock;
	unsigned int full_move_number;
	ChessSquare king_squares[3];
} ChessPosition;

typedef struct ChessMove {
	ChessSquare from;
	ChessSquare to;
	ChessPieceType promotion_type;
} ChessMove;

typedef struct ChessMoves {
	struct ChessMove moves[CHESS_MOVES_MAXIMUM_COUNT];
	size_t count;
} ChessMoves;

void chess_color_debug(ChessColor color);
bool chess_color_is_valid(ChessColor color);
ChessColor chess_color_opposite(ChessColor color);

void chess_piece_type_debug(ChessPieceType type);
bool chess_piece_type_is_valid(ChessPieceType type);
size_t chess_piece_type_from_algebraic(ChessPieceType *type, const char *string);
size_t chess_piece_type_to_algebraic(ChessPieceType type, char *string, size_t string_size);

void chess_piece_debug(ChessPiece piece);
bool chess_piece_is_valid(ChessPiece piece);
static inline ChessPiece chess_piece_new(ChessColor color, ChessPieceType type) {
	return (ChessPiece)(color << 3U | type);
}
static inline ChessColor chess_piece_color(ChessPiece piece) {
	return (ChessColor)(piece >> 3U);
}
static inline ChessPieceType chess_piece_type(ChessPiece piece) {
	return (ChessPieceType)(piece & 0x7U);
}
size_t chess_piece_from_algebraic(ChessPiece *piece, const char *string);
size_t chess_piece_to_algebraic(ChessPiece piece, char *string, size_t string_size);

void chess_file_debug(ChessFile file);
bool chess_file_is_valid(ChessFile file);
size_t chess_file_from_algebraic(ChessFile *file, const char *string);
size_t chess_file_to_algebraic(ChessFile file, char *string, size_t string_size);

void chess_rank_debug(ChessRank rank);
bool chess_rank_is_valid(ChessRank rank);
size_t chess_rank_from_algebraic(ChessRank *rank, const char *string);
size_t chess_rank_to_algebraic(ChessRank rank, char *string, size_t string_size);

void chess_square_debug(ChessSquare square);
bool chess_square_is_valid(ChessSquare square);
static inline ChessSquare chess_square_new(ChessFile file, ChessRank rank) {
	return (ChessSquare)(file | rank << 4U);
}
static inline ChessFile chess_square_file(ChessSquare square) {
	return (ChessFile)(square & 0xFU);
}
static inline ChessRank chess_square_rank(ChessSquare square) {
	return (ChessRank)(square >> 4U);
}
ChessColor chess_square_color(ChessSquare square);
size_t chess_square_from_algebraic(ChessSquare *square, const char *string);
size_t chess_square_to_algebraic(ChessSquare square, char *string, size_t string_size);
bool chess_square_is_attacked(const ChessPosition *position, ChessSquare square, ChessColor color);

bool chess_position_is_valid(const ChessPosition *position);
ChessPosition chess_position_new(void);
size_t chess_position_from_fen(ChessPosition *position, const char *string);
size_t chess_position_to_fen(const ChessPosition *position, char *string, size_t string_size);
bool chess_position_is_check(const ChessPosition *position);
bool chess_position_is_checkmate(const ChessPosition *position);
bool chess_position_is_stalemate(const ChessPosition *position);
bool chess_position_is_fifty_move_rule(const ChessPosition *position);
bool chess_position_is_threefold_repetition(const ChessPosition *position);
bool chess_position_is_insufficient_material(const ChessPosition *position);

void chess_move_debug(ChessMove move);
bool chess_move_is_valid(ChessMove move);
size_t chess_move_from_algebraic(const ChessPosition *position, ChessMove *move, const char *string);
size_t chess_move_to_algebraic(const ChessPosition *position, ChessMove move, char *string, size_t string_size);
bool chess_move_is_legal(const ChessPosition *position, ChessMove move);
bool chess_move_is_promotion(const ChessPosition *position, ChessMove move);
bool chess_move_is_en_passant(const ChessPosition *position, ChessMove move);
bool chess_move_is_capture(const ChessPosition *position, ChessMove move);
bool chess_move_is_kingside_castling(const ChessPosition *position, ChessMove move);
bool chess_move_is_queenside_castling(const ChessPosition *position, ChessMove move);
bool chess_move_do(ChessPosition *position, ChessMove move);

ChessMoves chess_moves_generate(const ChessPosition *position);

unsigned long chess_perft(unsigned int depth);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H
