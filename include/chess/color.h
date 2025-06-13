#ifndef CHESS_COLOR_H_INCLUDED
#define CHESS_COLOR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/macros.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stdint.h>

CHESS_ENUM(uint8_t, ChessColor) {
	CHESS_COLOR_WHITE = 0,
	CHESS_COLOR_BLACK = 1,

	CHESS_COLOR_NONE  = 2,
};

void chess_color_debug(ChessColor color);
bool chess_color_is_valid(ChessColor color);
ChessColor chess_color_opposite(ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_COLOR_H_INCLUDED
