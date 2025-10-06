#ifndef CHESS_POSITION_PRIVATE_H_INCLUDED
#define CHESS_POSITION_PRIVATE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/color.h>
#include <chess/position.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif

void chess_position_place_piece(ChessPosition *position, ChessPiece piece, ChessSquare square);
ChessPiece chess_position_remove_piece(ChessPosition *position, ChessSquare square);
void chess_position_move_piece(ChessPosition *position, ChessSquare from, ChessSquare to);
bool chess_position_is_king_attacked(const ChessPosition *position, ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_POSITION_PRIVATE_H_INCLUDED
