#ifndef CHESS_PIECE_TYPE_H_INCLUDED
#define CHESS_PIECE_TYPE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/macros.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

CHESS_ENUM(uint8_t, ChessPieceType) {
	CHESS_PIECE_TYPE_PAWN   = 0,
	CHESS_PIECE_TYPE_KNIGHT = 1,
	CHESS_PIECE_TYPE_BISHOP = 2,
	CHESS_PIECE_TYPE_ROOK   = 3,
	CHESS_PIECE_TYPE_QUEEN  = 4,
	CHESS_PIECE_TYPE_KING   = 5,

	CHESS_PIECE_TYPE_NONE   = 6,
};

void chess_piece_type_debug(ChessPieceType type);
bool chess_piece_type_is_valid(ChessPieceType type);
size_t chess_piece_type_from_algebraic(ChessPieceType *type, const char *string);
size_t chess_piece_type_to_algebraic(ChessPieceType type, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_PIECE_TYPE_H_INCLUDED
