#ifndef CHESS_MOVE_H_INCLUDED
#define CHESS_MOVE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

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

#ifdef __cplusplus
}
#endif

#endif // CHESS_MOVE_H_INCLUDED
