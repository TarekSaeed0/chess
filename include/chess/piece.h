#ifndef CHESS_PIECE_H_INCLUDED
#define CHESS_PIECE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

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

#ifdef __cplusplus
}
#endif

#endif // CHESS_PIECE_H_INCLUDED
