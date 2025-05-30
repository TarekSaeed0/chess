#ifndef CHESS_MOVES_H_INCLUDED
#define CHESS_MOVES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

ChessMoves chess_moves_generate(const ChessPosition *position);

#ifdef __cplusplus
}
#endif

#endif // CHESS_MOVES_H_INCLUDED
