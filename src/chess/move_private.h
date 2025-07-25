#ifndef CHESS_MOVE_PRIVATE_H_INCLUDED
#define CHESS_MOVE_PRIVATE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/move.h>
#include <chess/position.h>

void chess_move_do_unchecked(ChessPosition *position, ChessMove move);
void chess_move_undo_unchecked(ChessPosition *position, ChessMove move);

#ifdef __cplusplus
}
#endif

#endif // CHESS_MOVE_PRIVATE_H_INCLUDED
