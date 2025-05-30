#ifndef CHESS_PIECE_TYPE_H_INCLUDED
#define CHESS_PIECE_TYPE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

void chess_piece_type_debug(ChessPieceType type);
bool chess_piece_type_is_valid(ChessPieceType type);
size_t chess_piece_type_from_algebraic(ChessPieceType *type, const char *string);
size_t chess_piece_type_to_algebraic(ChessPieceType type, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_PIECE_TYPE_H_INCLUDED
