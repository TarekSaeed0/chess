#include <chess/color.h>

#include <assert.h>
#include <stdio.h>

void chess_color_debug(ChessColor color) {
	switch (color) {
		case CHESS_COLOR_NONE:
			printf("CHESS_COLOR_NONE");
			break;
		case CHESS_COLOR_WHITE:
			printf("CHESS_COLOR_WHITE");
			break;
		case CHESS_COLOR_BLACK:
			printf("CHESS_COLOR_BLACK");
			break;
		default:
			printf("(ChessColor)%d", color);
			break;
	}
}
bool chess_color_is_valid(ChessColor color) {
	switch (color) {
		case CHESS_COLOR_WHITE:
		case CHESS_COLOR_BLACK: return true;
		default: return false;
	}
}
ChessColor chess_color_opposite(ChessColor color) {
	assert(chess_color_is_valid(color) || color == CHESS_COLOR_NONE);

	switch (color) {
		case CHESS_COLOR_WHITE: return CHESS_COLOR_BLACK;
		case CHESS_COLOR_BLACK: return CHESS_COLOR_WHITE;
		default: return CHESS_COLOR_NONE;
	}
}
