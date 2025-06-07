#ifndef CHESS_CASTLING_RIGHTS_H_INCLUDED
#define CHESS_CASTLING_RIGHTS_H_INCLUDED

#include <chess/types.h>

void chess_castling_rights_debug(ChessCastlingRights rights);
bool chess_castling_rights_is_valid(ChessCastlingRights rights);

#endif // CHESS_CASTLING_RIGHTS_H_INCLUDED
