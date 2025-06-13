#ifndef CHESS_PIECE_H_INCLUDED
#define CHESS_PIECE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/color.h>
#include <chess/macros.h>
#include <chess/piece_type.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stdint.h>

CHESS_ENUM(uint8_t, ChessPiece) {
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

	CHESS_PIECE_NONE         = CHESS_COLOR_NONE << 3U | CHESS_PIECE_TYPE_NONE,
};

void chess_piece_debug(ChessPiece piece);
bool chess_piece_is_valid(ChessPiece piece);
ChessPiece chess_piece_new(ChessColor color, ChessPieceType type);
ChessColor chess_piece_color(ChessPiece piece);
ChessPieceType chess_piece_type(ChessPiece piece);
size_t chess_piece_from_algebraic(ChessPiece *piece, const char *string);
size_t chess_piece_to_algebraic(ChessPiece piece, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_PIECE_H_INCLUDED
