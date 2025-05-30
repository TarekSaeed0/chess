#ifndef CHESS_COLOR_H_INCLUDED
#define CHESS_COLOR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

void chess_color_debug(ChessColor color);
bool chess_color_is_valid(ChessColor color);
ChessColor chess_color_opposite(ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_COLOR_H_INCLUDED
