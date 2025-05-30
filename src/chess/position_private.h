#ifndef CHESS_POSITION_PRIVATE_H_INCLUDED
#define CHESS_POSITION_PRIVATE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

bool chess_position_is_king_attacked(const ChessPosition *position, ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_POSITION_PRIVATE_H_INCLUDED
