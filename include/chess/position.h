#ifndef CHESS_POSITION_H_INCLUDED
#define CHESS_POSITION_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

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

#ifdef __cplusplus
}
#endif

#endif // CHESS_POSITION_H_INCLUDED
