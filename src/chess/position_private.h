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

bool chess_position_is_king_attacked(const ChessPosition *position, ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_POSITION_PRIVATE_H_INCLUDED
